#include "Game.h"

Game::Game() : status(Game_status::WAITING) {}

/*
 * 1. Leer posibles comandos de todos los clientes
 *      a. try_pop a cada cliente
 *      b. mapear acciones
 *      c. ejecutar acciones (actualizar valores de lo que sea)
 * 2. Ejecutar un tick y guardar info actualizada (jugadores o enemigos moviendose, etc)
 *      a. recorrer pool de entities
 *      b. si se actualizó info, agregar a paquete de actualización
 * 3. Armar y distribuir paquete de actualización a todos los clientes
 * 4. sleep ????
 */
void Game::run() {
    status = Game_status::RUNNING;
    while (status == Game_status::RUNNING) {
        for (Client* client : Client_Monitor::getAll()) {
            uint8_t action = client->getReceiver().get_next_action();
            process_action(action, client->get_player_position());
        }

        std::vector<Update> total_updates;
        std::vector<Update> tick_updates;
        for (Dynamic_entity entity : entity_pool) {
            tick_updates = entity.tick();
            total_updates.insert(total_updates.end(), tick_updates.begin(), tick_updates.end());
        }

        Client_Monitor::sendAll(total_updates);

        // sleep ???
    }
}

void Game::process_action(uint8_t action, int player) {
    if (action == NULL_ACTION) {
        return;
    }
}
