#ifndef LOBBY_H
#define LOBBY_H

#include <optional>
#include <stdexcept>

#include "protocololobby.h"

class Lobby {
    LobbyProtocol* lobbyProtocol = nullptr;

    void checkProtocolOrError(const char* error);

   public:
    Lobby();

    int exec();

    void connectToServer(const std::string& ip, const std::string& port);
    void desconnectFromServer();

    std::vector<GameMatch> getServerMatches();

    void connectToMatch(u_int16_t id);

    uint16_t createMatch(const std::string& selected_map,
                         const std::string& matchName);

    Lobby(const Lobby&) = delete;
    Lobby& operator=(const Lobby&) = delete;

    ~Lobby();
};

#endif  // LOBBY_H
