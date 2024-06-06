#ifndef LOBBY_H
#define LOBBY_H

#include <stdexcept>

#include "protocololobby.h"

class Lobby {
    std::optional<LobbyProtocol> lobbyProtocol;

    LobbyProtocol& getProtocolOrError(char* error);

   public:
    Lobby();

    int exec();

    void connectToServer(const std::string& ip, const std::string& port);

    std::vector<GameMatch> getServerMatches();

    void connectToMatch(u_int16_t id);

    uint16_t createMatch(const std::string& selected_map, uint8_t playerCount,
                         std::string matchName);

    Lobby(const Lobby&) = delete;
    Lobby& operator=(const Lobby&) = delete;
};

#endif  // LOBBY_H
