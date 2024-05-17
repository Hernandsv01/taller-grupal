#ifndef PROTOCOLOLOBBY_H
#define PROTOCOLOLOBBY_H

#include "socket/socket.h"

#include <stdint.h>
#include <string>
#include <vector>

struct Partida {
    uint16_t id;
    uint8_t cantidad_necesaria_jugadores;
    uint8_t cantidad_jugadores_actuales;
    std::string nombre;
};

class ProtocoloLobby
{
    Socket socket;

public:
    ProtocoloLobby(const char* ip, const char* port);

    std::vector<Partida> obtener_partidas();

    void conectar_a_partida(u_int16_t id);

    //Devuelve el id de la partida creada
    uint16_t crear_partida(Partida partida);
};

#endif // PROTOCOLOLOBBY_H
