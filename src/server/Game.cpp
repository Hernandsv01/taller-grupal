#include "Game.h"

#include <algorithm>
#include <utility>

#define WAVE_INTERVAL_IN_SECONDS 10

Game::Game(std::string name, Map map)
    : Thread("Game server"),
      status(Game_status::WAITING),
      map(std::move(map)),
      name(name),
      next_id(0) {}

// Agregado para poder parar el loop del servidor, antes de joinear este thread
void Game::stop_custom() { status = Game_status::FINISHED; }

void Game::run() {
    status = Game_status::RUNNING;
    std::chrono::steady_clock::time_point current_tick_start =
        std::chrono::steady_clock::now();
    int time_left = Config::get_game_time();
    std::chrono::steady_clock::time_point next_second_update =
        current_tick_start + std::chrono::seconds(1);
    std::chrono::steady_clock::time_point next_wave = std::chrono::steady_clock::now() + std::chrono::seconds(WAVE_INTERVAL_IN_SECONDS);

    initialize_wave();

    while (status == Game_status::RUNNING) {
        std::chrono::steady_clock::time_point current_tick_end =
            current_tick_start + TICK_DURATION;

        run_iteration();
        delete_inactive_entities();

        // Calculate the start time for the next tick
        current_tick_start += TICK_DURATION;

        // Sleep until the next tick
        std::this_thread::sleep_until(current_tick_end);

        if (time_left < 0) {
            status = Game_status::FINISHED;
            sendAll({Update::Update_new::create_value(0, Update::MatchEnded, 0)});
        } else if (std::chrono::steady_clock::now() >= next_second_update) {
            time_left--;
            next_second_update += std::chrono::seconds(1);
            sendAll({Update::Update_new::create_value(
                static_cast<uint16_t>(0), Update::RemainingSeconds,
                static_cast<uint8_t>(time_left))});

            if (std::chrono::steady_clock::now() >= next_wave) {
                initialize_wave();
                next_wave = std::chrono::steady_clock::now() + std::chrono::seconds(WAVE_INTERVAL_IN_SECONDS);
            }
        }
    }
}

void Game::run_iteration() {
    std::vector<Update::Update_new> total_updates;
    std::vector<Update::Update_new> tick_updates;

    for (auto& client : clients) {
        uint8_t action = client->getReceiver().get_next_action();
        auto* player = dynamic_cast<Player*>(
            entity_pool[findEntityPositionById(entity_pool, client->get_id())].get());
        tick_updates = player->process_action(action, entity_pool, next_id);
        total_updates.insert(total_updates.end(), tick_updates.begin(),
                             tick_updates.end());
    }

    for (size_t i = 0; i < entity_pool.size(); ++i) {
        std::unique_ptr<Dynamic_entity>& entity_ptr = entity_pool[i];
        tick_updates = entity_ptr->tick(map, entity_pool, next_id);
        total_updates.insert(total_updates.end(), tick_updates.begin(),
                             tick_updates.end());
    }
    sendAll(total_updates);
}

void Game::delete_inactive_entities() {
    for (auto it = entity_pool.begin(); it != entity_pool.end();) {
        if ((*it)->isPendingDeletion()) {
            it = entity_pool.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::initialize_wave() {
    std::vector<Update::Update_new> creation_updates;
    std::vector<Update::Update_new> general_updates;

    std::array<Update::EntitySubtype, 3> enemy_subtypes = {
        Update::EntitySubtype::Enemy1, Update::EntitySubtype::Enemy2,
        Update::EntitySubtype::Enemy3};

    std::array<Update::EntitySubtype, 5> pickup_subtypes = {
        Update::EntitySubtype::Coin, Update::EntitySubtype::Carrot,
        Update::EntitySubtype::Light, Update::EntitySubtype::Heavy,
        Update::EntitySubtype::Power};

    // enemies
    std::vector<Coordinate> enemy_spawns = map.get_enemy_spawns();
    for (auto& enemy_spawn : enemy_spawns) {
        Update::EntitySubtype enemy_subtype = enemy_subtypes[rand() % enemy_subtypes.size()];
        Enemy enemy(next_id++, enemy_spawn.x, enemy_spawn.y, enemy_subtype);
        entity_pool.push_back(std::make_unique<Enemy>(enemy));

        creation_updates.push_back(Update::Update_new::create_create_entity(
            enemy.get_id(),
            Update::EntityType::Enemy,
            enemy_subtype
        ));
        general_updates.push_back(Update::Update_new::create_position(
            enemy.get_id(),
            enemy_spawn.x,
            enemy_spawn.y
        ));
    }

    // pickups
    std::vector<Coordinate> pickup_spawns = map.get_items_spawns();
    for (const auto& pickup_spawn : pickup_spawns) {
        Update::EntitySubtype pickup_subtype = pickup_subtypes[rand() % pickup_subtypes.size()];
        Pickup pickup(next_id++, pickup_spawn.x, pickup_spawn.y, static_cast<Pickup_type>(pickup_subtype), pickup_subtype);
        entity_pool.push_back(std::make_unique<Pickup>(pickup));

         creation_updates.push_back(Update::Update_new::create_create_entity(
             pickup.get_id(),
             Update::EntityType::Item,
             pickup_subtype
         ));
         general_updates.push_back(Update::Update_new::create_position(
             pickup.get_id(),
             pickup_spawn.x,
             pickup_spawn.y
         ));
    }

    sendAll(creation_updates);
    sendAll(general_updates);
}

std::vector<Update::Update_new> Game::get_full_game_updates() {
    std::vector<Update::Update_new> general_updates;
    std::vector<Update::Update_new> player_updates;
    for (const auto& entity : entity_pool) {
        uint16_t entity_id = entity->get_id();
        Update::EntityType entity_type;
        Update::EntitySubtype entity_subtype;
        if (auto player = dynamic_cast<Player*>(entity.get())) {
            entity_type = Update::EntityType::Player;
            entity_subtype = player->get_player_subtype();
            player_updates.push_back(Update::Update_new::create_value(
                    static_cast<uint16_t>(entity_id),
                    Update::UpdateType::Health,
                    static_cast<uint8_t>(player->get_health())));
        } else if (auto pickup = dynamic_cast<Pickup*>(entity.get())) {
            entity_type = Update::EntityType::Item;
            entity_subtype = pickup->get_subtype();
        } else if (auto enemy = dynamic_cast<Enemy*>(entity.get())) {
            entity_type = Update::EntityType::Enemy;
            entity_subtype = enemy->get_subtype();
        } else {
            continue;
        }

        general_updates.push_back(Update::Update_new::create_create_entity(
            entity_id, entity_type, entity_subtype));

        auto [x_client, y_client] = entity->get_position_for_client();

        general_updates.push_back(
            Update::Update_new::create_position(entity_id, x_client, y_client));
    }
    general_updates.insert(general_updates.end(), player_updates.begin(), player_updates.end());
    return general_updates;
}

uint16_t Game::add_player() {
    Coordinate rand_spawn =
        map.get_player_spawns()[rand() % map.get_player_spawns().size()];
    std::array<Update::EntitySubtype, 3> player_subtypes = {
        Update::EntitySubtype::Jazz, Update::EntitySubtype::Spaz,
        Update::EntitySubtype::Lori};
    Player player(next_id++, rand_spawn.x, rand_spawn.y,
                  player_subtypes[rand() % player_subtypes.size()]);
    entity_pool.push_back(std::make_unique<Player>(player));

    return player.get_id();
}

uint8_t Game::get_id() { return id; }

void Game::set_id(uint8_t game_id) { this->id = game_id; }

std::string Game::get_match_name() { return this->name; }

std::string Game::get_map_name() { return this->map.get_name(); }

void Game::add_socket_for_player(uint16_t player_id, Socket socket) {
    clients.push_back(std::make_unique<Server_Client>(
        static_cast<int>(player_id), std::move(socket)));

    // sendAll({Update::Update_new::create_create_entity(
    // player_id, Update::EntityType::Player, Update::EntitySubtype::Jazz)});

    // Send create existing entitys to this player
    std::vector<Update::Update_new> full_updates = get_full_game_updates();
    sendAll(full_updates);
}

void Game::delete_disconnected_players(
    std::vector<Update::Update_new>& updates) {
    std::vector<int> id_players_to_delete;

    // itera sobre clientes, buscando cuales se desconectaron.
    // Si encuentra alguno desconectado, lo elimina de la lista de clientes
    // y agrega su id a la lista de ids a eliminar
    for (auto it = clients.begin(); it != clients.end();) {
        if ((*it)->has_desconnected()) {
            id_players_to_delete.push_back((*it)->get_id());
            it = clients.erase(it);
        } else {
            ++it;
        }
    }

    // Itera la lista de ids eliminados. Agrega a las updates la update de
    // entidad eliminada y luego elimina la entidad de la entity_pool.
    for (int id : id_players_to_delete) {
        std::cout << "Delete player id: " << std::to_string(id) << std::endl;

        updates.push_back(Update::Update_new::create_delete_entity(
            static_cast<uint16_t>(id)));

        int entity_position = findEntityPositionById(entity_pool, id);
        if (entity_position != -1) {
            entity_pool.erase(entity_pool.begin() + entity_position);
        }
    }
}

void Game::sendAll(std::vector<Update::Update_new> updates) {
    delete_disconnected_players(updates);

    for (auto& i : clients) {
        i->getSender().addToQueue(updates);
    }
}

int Game::findEntityPositionById(std::vector<std::unique_ptr<Dynamic_entity>>& entities, int entity_id) {
    auto it = std::find_if(
            entities.begin(), entities.end(),
        [entity_id](const std::unique_ptr<Dynamic_entity>& entity) {
            return entity->get_id() == entity_id;
        });
    if (it != entities.end()) {
        return std::distance(entities.begin(), it);
    }
    return -1;
}
