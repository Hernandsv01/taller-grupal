#ifndef PROTOCOLOLOBBYCLIENT_H
#define PROTOCOLOLOBBYCLIENT_H

#include <stdint.h>

#include <string>
#include <vector>

#include "../common/lobbyprotocol.h"
#include "socket/socket.h"

// (desde punto de vista del cliente)
// - Crear partida:
//   - envía: Nombre de partida, mapa.
//   - recive: id partida.

// - Unirse partida:
//   - envia: id partida.
//   - recive: id_jugador (antes, despues, Mensaje de creacion de entidades. No
//   se si es del manager, del gameloop, o de quien), mapa.

// - Obtener lista partidas:
//   - envía: nada?
//   - recive: vector de ids de partidas, y nombres de esas partidas.

// Bytes de los mensajes:
// - CREATE
//   - envia:
//     - magic number (uint8_t)
//     - bytes de letras (sizeof(char)*16)
//     - Mapa ¿Como lo enviamos? (POR AHORA SIN IMPLEMENTAR)

//   - recive:
//     - id_partida (uint16_t)

// - JOIN
//  - envia:
//     - magic number (uint8_t)
//     - id_partida (uint16_t)

//  - recibe:
//     - id_jugador (uint16_t)
//     - Mapa ¿Como lo enviamos? (POR AHORA SIN IMPLEMENTAR)
//     - ¿Todas las updates pertinentes para inicializar partida?

// - GET_GAMES
//   - envia:
//     - magic number (uint8_t)

//   - recibe:
//     - cantidad_partidas (uint8_t)
//     - vector:
//       - id_partida (uint8_t)
//       - nombre_partida (sizeof(char)*16)

// class LobbyProtocol {
//     Socket socket;

//    public:
//     LobbyProtocol(const char* ip, const char* port);

//     std::vector<GameMatch> getGameMatches();

//     void connectToMatch(u_int16_t id);

//     // Devuelve el id de la partida creada
//     uint16_t createMatch(GameMatch match);
// };

class LobbyProtocol {
    Socket socket;

   public:
    LobbyProtocol(const char* ip, const char* port);

    match_id createMatch(const std::string& match_name) {
        if (match_name.length() > 16) {
            throw std::runtime_error(
                "El nombre de la partida es demasiado largo. Maximo 16"
                "caracteres.");
        }

        MessageType type = CREATE;
        socket.sendall(&type, sizeof(type));

        uint8_t buffer[16] = {0};
        memcpy(buffer, match_name.c_str(), match_name.length());

        socket.sendall(buffer, sizeof(buffer));

        match_id id;
        socket.recvall(&id, sizeof(id));

        return id;
    }

    uint16_t joinMatch(match_id id) {
        MessageType type = JOIN;
        socket.sendall(&type, sizeof(type));

        socket.sendall(&id, sizeof(id));

        // Recibir id jugador
        uint16_t player_id;
        socket.recvall(&player_id, sizeof(player_id));
        player_id = ntohs(player_id);

        // Recibir mapa (CUANDO DECIDAMOS)

        return player_id;
    }

    std::vector<GameMatch> getGameMatches() {
        MessageType type = GET_GAMES;
        socket.sendall(&type, sizeof(type));

        uint8_t games_count;
        socket.recvall(&games_count, sizeof(games_count));

        std::vector<GameMatch> games(games_count);
        for (uint8_t i = 0; i < games_count; i++) {
            match_id id;
            socket.recvall(&id, sizeof(id));

            char buffer[17] = {0};
            socket.recvall(buffer, sizeof(buffer) - 1);

            games[i] = GameMatch{id, std::string(buffer)};
        }

        return games;
    }
};

#endif  // PROTOCOLOLOBBY_H
