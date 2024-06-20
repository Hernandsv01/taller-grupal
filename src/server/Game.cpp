#include "Game.h"

#include <utility>

#define GAME_LENGTH_IN_SECONDS 200

Game::Game(std::string name, Map map) : Thread("Game server"), status(Game_status::WAITING), map(std::move(map)), name(name), next_id(0) {
    // Hardcodeado para que se asocie un jugador al único cliente que se conecta

    entity_pool.push_back(std::make_unique<Player>(0, 0, 0));
    //mapa

}

// Agregado para poder parar el loop del servidor, antes de joinear este thread
void Game::stop_custom() { status = Game_status::FINISHED; }

void Game::run() {
    status = Game_status::RUNNING;
    std::chrono::steady_clock::time_point current_tick_start = std::chrono::steady_clock::now();
    int time_left = GAME_LENGTH_IN_SECONDS;
    std::chrono::steady_clock::time_point next_second_update = current_tick_start + std::chrono::seconds(1);

    while (status == Game_status::RUNNING) {
        std::chrono::steady_clock::time_point current_tick_end = current_tick_start + TICK_DURATION;

        run_iteration();

        // Calculate the start time for the next tick
        current_tick_start += TICK_DURATION;

        // Sleep until the next tick
        std::this_thread::sleep_until(current_tick_end);

        if (time_left < 0) {
            status = Game_status::FINISHED;
            Client_Monitor::sendAll({Update::Update_new::create_value(0, Update::MatchEnded, 0)});
        } else if (std::chrono::steady_clock::now() >= next_second_update) {
            time_left--;
            next_second_update += std::chrono::seconds(1);
            Client_Monitor::sendAll({Update::Update_new::create_value(static_cast<uint16_t>(0), Update::RemainingSeconds, static_cast<uint8_t>(time_left))});
        }
    }

    // TODO: Send final stats
}

void Game::run_iteration() {
    std::vector<Update::Update_new> total_updates;
    std::vector<Update::Update_new> tick_updates;

    for (Server_Client* client : Client_Monitor::getAll()) {
        uint8_t action = client->getReceiver().get_next_action();
        auto* player = dynamic_cast<Player*>(entity_pool[client->get_player_position()].get());
        tick_updates = player->process_action(action, entity_pool, next_id);
        total_updates.insert(total_updates.end(), tick_updates.begin(),tick_updates.end());
    }

    for (std::unique_ptr<Dynamic_entity>& entity_ptr : entity_pool) {
        tick_updates = entity_ptr->tick(map, entity_pool);
        total_updates.insert(total_updates.end(), tick_updates.begin(),tick_updates.end());
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

void Game::add_socket_for_player(uint16_t player_id, Socket socket) {
    // Acá se destruye el socket, por lo que el cliente no puede continuar, y no
    // hace nada
}
