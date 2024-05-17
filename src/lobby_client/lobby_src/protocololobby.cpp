#include "protocololobby.h"

#include <stdexcept>
#include <arpa/inet.h>
#include <iostream>

ProtocoloLobby::ProtocoloLobby(const char* ip,const char* puerto): socket(ip, puerto) {

}

std::vector<Partida> ProtocoloLobby::obtener_partidas() {
    // TODO: MODIFICAR PROTOCOLO

    // TODO: ENVIAR PETICION PARTIDAS
    char peticion[] = "Solicito partidas (4 bytes)";
    socket.sendall(peticion, sizeof(peticion)-1);

    char contenido[5] = {0};

    socket.recvall(contenido, 4);

    std::string string_partida(contenido);
    std::vector<Partida> partidas;

    for (uint16_t i = 0; i < 5; i++) {
        Partida partida = {i, 3, 0,string_partida + std::to_string(i)};

        partidas.push_back(partida);
    }

    return partidas;
}

void ProtocoloLobby::conectar_a_partida(u_int16_t id) {
    std::string peticion = "Conectarse a partida " + std::to_string(id) + ". Responder 1 byte";

    socket.sendall((char*)peticion.c_str(), peticion.length());

    uint8_t respuesta;
    socket.recvall(&respuesta, 1);

    if (respuesta == 0) {
        throw std::runtime_error("No se pudo conectar a partida");
    }
}

uint16_t ProtocoloLobby::crear_partida(Partida partida) {
    std::string peticion = "Solicito crear partida para " + std::to_string(partida.cantidad_necesaria_jugadores) +
            " con nombre de partida " + partida.nombre;

    socket.sendall((char*)peticion.c_str(), peticion.length());

    uint16_t respuesta;
    socket.recvall(&respuesta, 2);

    respuesta = ntohs(respuesta);

    if (respuesta == 0) {
        throw std::runtime_error("No se pudo crear la partida");
    }

    std::cout << respuesta << std::endl;
    return respuesta;
}
