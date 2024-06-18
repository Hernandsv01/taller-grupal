#include "Game.h"

#include <utility>

Game::Game(std::string name, Map map) : Thread("Game server"), status(Game_status::WAITING), map(std::move(map)), name(name) {
    // Hardcodeado para que se asocie un jugador al único cliente que se conecta

    entity_pool.push_back(std::make_unique<Player>(0, 0, 0));
    //mapa

}

// Agregado para poder parar el loop del servidor, antes de joinear este thread
void Game::stop_custom() { status = Game_status::STOPPED; }

void Game::run() {
    status = Game_status::RUNNING;
    std::chrono::steady_clock::time_point inicio_tick = std::chrono::steady_clock::now();
    while (status == Game_status::RUNNING) {
        std::chrono::steady_clock::time_point final_tick =
            inicio_tick + TICK_DURATION;

        run_iteration();

        // Calculate the start time for the next tick
        inicio_tick += TICK_DURATION;

        // Sleep until the next tick
        std::this_thread::sleep_until(final_tick);
    }
}

void Game::run_iteration() {
    for (Server_Client* client : Client_Monitor::getAll()) {
        uint8_t action = client->getReceiver().get_next_action();
        auto* player = dynamic_cast<Player*>(entity_pool[client->get_player_position()].get());
        player->process_action(action);
    }

    std::vector<Update::Update_new> total_updates;
    std::vector<Update::Update_new> tick_updates;
    for (std::unique_ptr<Dynamic_entity>& entity_ptr : entity_pool) {
        tick_updates = entity_ptr->tick(map, &entity_pool);
        total_updates.insert(total_updates.end(), tick_updates.begin(),
                             tick_updates.end());
    }

    Client_Monitor::sendAll(total_updates);
}

uint16_t Game::add_player() {
    uint16_t id_player = entity_pool.size();
    entity_pool.push_back(std::make_unique<Player>(id_player, 0, 0));
    return id_player;
}

uint8_t Game::get_id() {
    return id;
}

void Game::set_id(uint8_t game_id) {
    this->id = game_id;
}

std::string Game::get_match_name() {
    return this->name;
}

std::string Game::get_map_name(){
    return this->map.get_name();
}


