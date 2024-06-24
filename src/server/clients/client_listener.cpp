#include "client_listener.h"

void Client_listener::run() {
    while (is_running) {
        try {
            Socket skt = skt_listener.accept();

            // TODO: En algun momento debería recorrer todos los lobbys para
            // verificar si hay que eliminar alguno que haya terminado
            clientes_en_lobby.push_back(std::make_unique<LobbyManager>(
                std::move(skt), gamePoolMonitor));

            clientes_en_lobby.back()->start();

            joinEndedLobbyManagers();

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

void Client_listener::joinEndedLobbyManagers() {
    for (auto it = clientes_en_lobby.begin(); it != clientes_en_lobby.end();) {
        LobbyManager* lobbyThread = (*it).get();

        if (!lobbyThread->has_ended()) {
            // No necesito stopearlo porque ya terminó.
            lobbyThread->join();
            it = clientes_en_lobby.erase(it);
        } else {
            ++it;
        }
    }
}