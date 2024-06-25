#ifndef SERVER_CLIENT_LISTENER_H
#define SERVER_CLIENT_LISTENER_H

#include <utility>

#include "../../common/library/socket.h"
#include "../../common/library/thread.h"
// TODO: RENOMBRAR A LOBBY_MANAGER
#include "../game/lobby_manager.h"
#include "server_client.h"

#define SHUTDOWN_MODE 2

class Client_listener : public Thread {
   private:
    Socket& skt_listener;
    bool is_running;
    GamePoolMonitor& gamePoolMonitor;
    std::vector<std::unique_ptr<LobbyManager>> clientes_en_lobby;

    void joinEndedLobbyManagers();

   public:
    Client_listener(Socket& socket, GamePoolMonitor& gamePoolMonitor)
        : Thread("Client_listener server"),
          skt_listener(socket),
          is_running(true),
          gamePoolMonitor(gamePoolMonitor) {}

    void run() override;
    void kill();
};

#endif  // SERVER_CLIENT_LISTENER_H
