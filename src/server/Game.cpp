#include "Game.h"

Game::Game() : Thread("Game server"), status(Game_status::WAITING) {
    entity_pool.push_back(std::make_unique<Player>(0, 0, 0));
}

// Agregado para poder parar el loop del servidor, antes de joinear este thread
void Game::stop_custom() { status = Game_status::STOPPED; }

void Game::run() {
    status = Game_status::RUNNING;
    std::chrono::steady_clock::time_point INICIO_ABSOLUTO = reloj.now();
    std::chrono::steady_clock::time_point inicio_tick = INICIO_ABSOLUTO;
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
        process_action(action, client->get_player_position());
    }

    std::vector<Update> total_updates;
    std::vector<Update> tick_updates;
    for (std::unique_ptr<Dynamic_entity>& entity_ptr : entity_pool) {
        tick_updates = entity_ptr->tick(&entity_pool);
        total_updates.insert(total_updates.end(), tick_updates.begin(),
                             tick_updates.end());
    }

    Client_Monitor::sendAll(total_updates);
}

void Game::process_action(uint8_t action, int player) {
    if (action == NULL_ACTION) {
        return;
    }
    if (action == JUMP) {
        return;  // Not implemented
    }
    if (action == RUN_LEFT) {
        entity_pool[player]->setXSpeed(-1);
    }
    if (action == RUN_RIGHT) {
        entity_pool[player]->setXSpeed(1);
    }
    if (action == SHOOT) {
        return;  // Not implemented
    }
    if (action == SPECIAL) {
        return;  // Not implemented
    }
    if (action == STOP_RUN_RIGHT || action == STOP_RUN_LEFT) {
        entity_pool[player]->setXSpeed(0);
    }
}
