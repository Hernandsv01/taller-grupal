#ifndef LOBBY_H
#define LOBBY_H

#include "protocololobby.h"
#include <stdexcept>

class Lobby
{
    std::optional<ProtocoloLobby> protocolo;

    ProtocoloLobby& obtener_protocolo_o_error(char* error);

public:
    Lobby();

    int exec();

    void conectar_a_servidor(const std::string& ip, const std::string& puerto);

    std::vector<Partida> obtener_partidas_servidor();

    void conectarse_a_partida(u_int16_t id);

    uint16_t crear_partida(std::string mapa_selecionado, uint8_t cant_jugadores, std::string nombre_partida);

    Lobby(const Lobby&) = delete;
    Lobby& operator=(const Lobby&) = delete;

};

#endif // LOBBY_H
