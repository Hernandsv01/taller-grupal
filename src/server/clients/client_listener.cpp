#include "client_listener.h"

void Client_listener::run() {
    while (is_running) {
        try {
            Socket skt = skt_listener.accept();

            // TODO: En algun momento debería recorrer todos los lobbys para
            // verificar si hay que eliminar alguno que haya terminado
            clientes_en_lobby.push_back(std::make_unique<LobbyManager>(
                std::move(skt), gamePoolMonitor));

        } catch (ClosedConnectionError& e) {
            // Se cerró manualmente el listener para terminar el hilo.
            is_running = false;
        }
    }
}

void Client_listener::kill() {
    is_running = false;
    skt_listener.close_and_shutdown();
}
