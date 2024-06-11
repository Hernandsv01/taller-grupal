#ifndef PROTOCOLOLOBBYCLIENT_H
#define PROTOCOLOLOBBYCLIENT_H

#include <stdint.h>

#include <string>
#include <vector>

#include "../common/library/socket.h"
#include "../common/lobbyprotocol.h"

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

    match_id createMatch(const std::string& match_name);

    uint16_t joinMatch(match_id id);

    std::vector<GameMatch> getGameMatches();
};

#endif  // PROTOCOLOLOBBY_H
