#include "Game.h"

#include <utility>
#include "./loader/config.h"

#define GAME_LENGTH_IN_SECONDS 200

Game::Game(std::string name, Map map)
    : Thread("Game server"),
      status(Game_status::WAITING),
      map(std::move(map)),
      name(name),
      next_id(0) {
    // Hardcodeado para que se asocie un jugador al único cliente que se conecta

    // entity_pool.push_back(std::make_unique<Player>(0, 0, 0));
    // mapa
}

// Agregado para poder parar el loop del servidor, antes de joinear este thread
void Game::stop_custom() { status = Game_status::FINISHED; }

void Game::run() {
    status = Game_status::RUNNING;
    std::chrono::steady_clock::time_point current_tick_start =
        std::chrono::steady_clock::now();
    int time_left = GAME_LENGTH_IN_SECONDS;
    std::chrono::steady_clock::time_point next_second_update =
        current_tick_start + std::chrono::seconds(1);

    send_initial_values();

    while (status == Game_status::RUNNING) {
        std::chrono::steady_clock::time_point current_tick_end =
            current_tick_start + TICK_DURATION;

        run_iteration();

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

    // TODO: Send final stats
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

    for (std::unique_ptr<Dynamic_entity>& entity_ptr : entity_pool) {
        tick_updates = entity_ptr->tick(map, entity_pool, next_id);
        total_updates.insert(total_updates.end(), tick_updates.begin(),
                             tick_updates.end());
    }
    sendAll(total_updates);
}





void Game::send_initial_values() {

    std::vector<Update::Update_new> creations;
    std::vector<Update::Update_new> creations;
    //
    /*
        Reveisar orden: 
        1. crear entidades
            a. usar next_id para settear id (next_id++ luego)
            b. usar métodos de spawn de mapa para ver dónde crearlas
        2. meterlas en la entity_pool
        3. Crear update de creación de esas entidades
        4. Mandar paquete de updates de creación a todos los clientes
        5. Crear update para poner cada entidad en su posición inicial
        6. Mandar paquete de updates
    */

    //player
    Coordinate rand_spawn = map.get_player_spawns()[rand() % map.get_player_spawns().size()];
   
    Player player(next_id++, rand_spawn.x, rand_spawn.y);
    entity_pool.push_back(std::make_unique<Player>(player));
    // 
    Update::Update_new::create_create_entity(
        player.get_id(),
        Update::EntityType::Player,
        Update::EntitySubtype::Jazz //deberia ser random entre los 3 tipos. 
    );

    Update::Update_new::create_position(
        player.get_id(), // usar mismo id que en la creación
        rand_spawn.x,
        rand_spawn.y
    );
    

    Update::Update_new::create_value(player.get_id, )


    //enemies
    Coordinate rand_enemy_spawn = map.get_enemy_spawns()[rand() % map.get_enemy_spawns().size()];
    /*
    //se deberia chequear al cantidad de enemigos. 
    Enemy enemy(next_id++, rand_enemy_spawn.x, rand_enemy_spawn.y);
    Update::Update_new::create_create_entity(
        enemy.get_id(),
        Update::EntityType::Enemy,
        Update::EntitySubtype::Enemy1 //deberia ser random entre los tres tipos. 
    
    );
    Update::Update_new::create_position(
        enemy.get_id(), // usar mismo id que en la creación
        rand_spawn.x,
        rand_spawn.y
    );
    */
   
    //pickups
    //ver como se maneja la cantidad 
    //ver como hacer para tomar la configuracion, si lo hago yo desde aca o si se hace desde el constructor. 
    Coordinate rand_pickup_spawn = map.get_items_spawns()[rand() % map.get_items_spawns().size()];
    Pickup pickup(next_id++, rand_pickup_spawn.x, rand_pickup_spawn.y, Pickup_type::COIN, Config::get_pickup_coin());  
    entity_pool.push_back(std::make_unique<Pickup>(pickup));

    Coordinate rand_pickup_spawn = map.get_items_spawns()[rand() % map.get_items_spawns().size()];
    Pickup pickup(next_id++, rand_pickup_spawn.x, rand_pickup_spawn.y, Pickup_type::COIN, Config::get_pickup_coin());  
    entity_pool.push_back(std::make_unique<Pickup>(pickup));


}

uint16_t Game::add_player() {
    //Cuando se suma un jugador a una partida, es random? 
    //Coordinate rand_spawn = map.get_player_spawns()[rand() % map.get_player_spawns().size()];
    //Player player(next_id++, rand_spawn.x, rand_spawn.y);
    Player player(next_id++, 4, 4);
    entity_pool.push_back(std::make_unique<Player>(player));
    return player.get_id();
}

uint8_t Game::get_id() { return id; }

void Game::set_id(uint8_t game_id) { this->id = game_id; }

std::string Game::get_match_name() { return this->name; }

std::string Game::get_map_name() { return this->map.get_name(); }

void Game::add_socket_for_player(uint16_t player_id, Socket socket) {
    clients.push_back(std::make_unique<Server_Client>(static_cast<int>(player_id), std::move(socket)));

    sendAll({Update::Update_new::create_create_entity(
        player_id, Update::EntityType::Player, Update::EntitySubtype::Jazz)});

    // Send create existing entitys to this player
}

void Game::sendAll(std::vector<Update::Update_new> updates) {
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
