#ifndef PROTOCOLOLOBBY_H
#define PROTOCOLOLOBBY_H

#include <stdint.h>

#include <string>
#include <vector>

// Incluyo este duplicado de socket, porque es una implementacion ligeramente
// modificada. Todavía hay que unificarlo.
#include "socket/socket.h"

struct GameMatch {
    uint16_t id;
    uint8_t requiredPlayersCount;
    uint8_t currentPlayerCount;
    std::string name;
};

class LobbyProtocol {
    Socket socket;

   public:
    LobbyProtocol(const char* ip, const char* port);

    std::vector<GameMatch> getGameMatches();

    void connectToMatch(u_int16_t id);

    // Devuelve el id de la partida creada
    uint16_t createMatch(GameMatch match);
};

#endif  // PROTOCOLOLOBBY_H
