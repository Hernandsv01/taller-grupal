#include <iostream>

#include "../common/library/socket.h"
#include "../common/lobbyprotocol.h"

// Bytes de los mensajes:
// - CREATE
//   - recibe:
//     - magic number (uint8_t)
//     - nombre_partida (sizeof(char)*16) (no es necesario enviar nullbyte)
//     - Mapa (probablemente sizeof(char)*32 (no es necesario enviar nullbyte))

//   - envia:
//     - id_partida (uint8_t)

// - JOIN
//  - recive:
//     - magic number (uint8_t)
//     - id_partida (uint8_t)

//  - envia:
//     - id_jugador (uint16_t)
//     - Mapa (probablemente sizeof(char)*32 (no es necesario enviar nullbyte))
//     - Todas las updates pertinentes para inicializar partida

// - GET_GAMES
//   - recibe:
//     - magic number (uint8_t)

//   - envia:
//     - cantidad_partidas (uint8_t)
//     - vector:
//       - id_partida (uint8_t)
//       - nombre_partida (sizeof(char)*16) (no es necesario enviar nullbyte)
//       - Mapa (probablemente sizeof(char)*32 (no es necesario enviar nullbyte)

int main() {
    std::vector<GameMatch> games;

    uint8_t id_partida = 4;

    games.emplace_back(GameMatch{0, "Partida 1", "Mapa 1"});
    games.emplace_back(GameMatch{1, "Otrapartida 2", "mapacio 2"});
    games.emplace_back(GameMatch{2, "Pirpiripip 3", "pirip 3"});
    games.emplace_back(GameMatch{3, "Sunguyrule 4", "sarac 4"});

    Socket socket("15500");

    Socket conexion = socket.accept();

    // ACTUA COMO SERVIDOR

    while (true) {
        MessageType type;

        conexion.recvall(&type, sizeof(type));

        if (type == MessageType::CREATE) {
            // Recibir los datos del mensaje CREATE

            char nombrePartida[16];
            char mapa[32];

            conexion.recvall(nombrePartida, sizeof(nombrePartida));
            conexion.recvall(mapa, sizeof(mapa));

            // Procesar los datos recibidos
            games.emplace_back(GameMatch{id_partida, nombrePartida, mapa});

            // Enviar la respuesta
            conexion.sendall(&id_partida, sizeof(id_partida));

            id_partida++;
        } else if (type == MessageType::JOIN) {
            // Recibir los datos del mensaje JOIN
            uint8_t idPartida;
            conexion.recvall(&idPartida, sizeof(idPartida));

            // Procesar los datos recibidos

            // Enviar la respuesta
            uint16_t idJugador =
                456;  // Obtener el ID del jugador que se unió a la partida

            idJugador = htons(idJugador);

            char mapa[32] = {0};
            memcpy(mapa, games[idPartida].map.c_str(), sizeof(mapa));

            conexion.sendall(&idJugador, sizeof(idJugador));
            conexion.sendall(mapa, sizeof(mapa));
            // Enviar todas las actualizaciones pertinentes para inicializar la
            // partida
        } else if (type == MessageType::GET_GAMES) {
            // Procesar el mensaje

            // Enviar la respuesta
            uint8_t cantidadPartidas = games.size();
            conexion.sendall(&cantidadPartidas, sizeof(cantidadPartidas));
            for (const auto& game : games) {
                conexion.sendall(&game.id, sizeof(game.id));

                char nombre_partida[16] = {0};
                memcpy(nombre_partida, game.name.c_str(),
                       sizeof(nombre_partida));

                conexion.sendall(nombre_partida, sizeof(nombre_partida));

                char mapa[32] = {0};
                memcpy(mapa, game.map.c_str(), sizeof(mapa));

                conexion.sendall(mapa, sizeof(mapa));
            }
        }
    }
}
