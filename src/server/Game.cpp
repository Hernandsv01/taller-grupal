#include "Game.h"

#include <algorithm>
#include <utility>

#include "./loader/config.h"

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

    initialize_values();

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
            sendAll(
                {Update::Update_new::create_value(0, Update::MatchEnded, 0)});
        } else if (std::chrono::steady_clock::now() >= next_second_update) {
            time_left--;
            next_second_update += std::chrono::seconds(1);
            sendAll({Update::Update_new::create_value(
                static_cast<uint16_t>(0), Update::RemainingSeconds,
                static_cast<uint8_t>(time_left))});
        }
    }
}

void Game::run_iteration() {
    std::vector<Update::Update_new> total_updates;
    std::vector<Update::Update_new> tick_updates;

    for (auto& client : clients) {
        uint8_t action = client->getReceiver().get_next_action();
        auto* player = dynamic_cast<Player*>(
            entity_pool[findEntityPositionById(client->get_id())].get());
        tick_updates = player->process_action(action, entity_pool, next_id);
        total_updates.insert(total_updates.end(), tick_updates.begin(),
                             tick_updates.end());
    }

    for (size_t i = 0; i < entity_pool.size(); ++i) {
        std::unique_ptr<Dynamic_entity>& entity_ptr = entity_pool[i];
        tick_updates = entity_ptr->tick(map, entity_pool, next_id);
        total_updates.insert(total_updates.end(), tick_updates.begin(), tick_updates.end());
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

void Game::initialize_values() {
    std::vector<Update::Update_new> creation_updates;
    std::vector<Update::Update_new> general_updates;

    std::array<Update::EntitySubtype, 3> player_subtypes = {
        Update::EntitySubtype::Jazz, Update::EntitySubtype::Spaz,
        Update::EntitySubtype::Lori};

    std::array<Update::EntitySubtype, 3> enemy_subtypes = {
        Update::EntitySubtype::Enemy1, Update::EntitySubtype::Enemy2,
        Update::EntitySubtype::Enemy3};

    std::array<Update::EntitySubtype, 5> pickup_subtypes = {
        Update::EntitySubtype::Coin, Update::EntitySubtype::Carrot,
        Update::EntitySubtype::Light, Update::EntitySubtype::Heavy,
        Update::EntitySubtype::Power};

    // player
    // Coordinate rand_spawn =
    //     map.get_player_spawns()[rand() % map.get_player_spawns().size()];
    // std::cout << std::to_string(rand_spawn.x) << std::endl;
    // std::cout << std::to_string(rand_spawn.y) << std::endl;
    // std::cout << "-------" << std::endl;
    // Player player(next_id++, rand_spawn.x, rand_spawn.y,
    //               player_subtypes[rand() % player_subtypes.size()]);
    // entity_pool.push_back(std::make_unique<Player>(player));

    // creation_updates.push_back(Update::Update_new::create_create_entity(
    //     player.get_id(),
    //     Update::EntityType::Player,
    //     player.get_player_subtype()
    // ));
    // std::cout << "Update de creación de player agregada" << std::endl;

    // general_updates.push_back(Update::Update_new::create_position(
    //     player.get_id(),
    //     rand_spawn.x,
    //     rand_spawn.y
    // ));
    // std::cout << "Update de posición de player agregada" << std::endl;

    // enemies
    std::cout << std::to_string(map.get_enemy_spawns().size()) << std::endl;
    std::cout << "<------->" << std::endl;
    if (map.get_enemy_spawns().size() != 0) {
        std::vector<Coordinate> enemy_spawns = map.get_enemy_spawns();
        int enemy_spawn_size = enemy_spawns.size();
        int rand_value = rand();
        int spawn_pos_selected = rand_value % enemy_spawn_size;
        Coordinate rand_enemy_spawn =
            map.get_enemy_spawns()[spawn_pos_selected];
        // se deberia chequear al cantidad de enemigos.
        // Config::get_enemy_count...
        Update::EntitySubtype enemy_subtype =
            enemy_subtypes[rand() % enemy_subtypes.size()];
        Enemy enemy(next_id++, rand_enemy_spawn.x, rand_enemy_spawn.y,
                    enemy_subtype);
        entity_pool.push_back(std::make_unique<Enemy>(enemy));
        // creation_updates.push_back(Update::Update_new::create_create_entity(
        //     enemy.get_id(),
        //     Update::EntityType::Enemy,
        //     enemy_subtype
        // ));
        // general_updates.push_back(Update::Update_new::create_position(
        //     enemy.get_id(),
        //     rand_spawn.x,
        //     rand_spawn.y
        // ));
    }

    // pickups
    for (const auto& pickup_spawn : map.get_items_spawns()) {
        Update::EntitySubtype pickup_subtype =
            pickup_subtypes[rand() % pickup_subtypes.size()];
        Pickup_type type = static_cast<Pickup_type>(pickup_subtype);
        Pickup pickup(next_id++, pickup_spawn.x, pickup_spawn.y, type,
                      pickup_subtype);
        entity_pool.push_back(std::make_unique<Pickup>(pickup));

        // creation_updates.push_back(Update::Update_new::create_create_entity(
        //     pickup.get_id(),
        //     Update::EntityType::Item,
        //     pickup_subtype
        // ));

        // general_updates.push_back(Update::Update_new::create_position(
        //     pickup.get_id(),
        //     pickup_spawn.x,
        //     pickup_spawn.y
        // ));
    }

    // sendAll(creation_updates);
    // sendAll(general_updates);
}

std::vector<Update::Update_new> Game::get_full_game_updates() {
    std::vector<Update::Update_new> updates;
    for (const auto& entity : entity_pool) {
        uint16_t entity_id = entity->get_id();
        Update::EntityType entity_type;
        Update::EntitySubtype entity_subtype;
        float position_x;
        float position_y;
        if (auto player = dynamic_cast<Player*>(entity.get())) {
            entity_type = Update::EntityType::Player;
            entity_subtype = player->get_player_subtype();
            position_x = player->getXPos();
            position_y = player->getYPos();
        } else if (auto pickup = dynamic_cast<Pickup*>(entity.get())) {
            entity_type = Update::EntityType::Item;
            entity_subtype = pickup->get_subtype();
            position_x = pickup->getXPos();
            position_y = pickup->getYPos();
        } else if (auto enemy = dynamic_cast<Enemy*>(entity.get())) {
            entity_type = Update::EntityType::Enemy;
            entity_subtype = enemy->get_subtype();
            position_x = enemy->getXPos();
            position_y = enemy->getYPos();
        } else {
            continue;
        }

        updates.push_back(Update::Update_new::create_create_entity(
            entity_id, entity_type, entity_subtype));

        updates.push_back(Update::Update_new::create_position(
            entity_id, position_x, position_y));
    }
    return updates;
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

        int entity_position = findEntityPositionById(id);
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

int Game::findEntityPositionById(int entity_id) {
    auto it = std::find_if(
        entity_pool.begin(), entity_pool.end(),
        [entity_id](const std::unique_ptr<Dynamic_entity>& entity) {
            return entity->get_id() == entity_id;
        });
    if (it != entity_pool.end()) {
        return std::distance(entity_pool.begin(), it);
    }
    return -1;
}
