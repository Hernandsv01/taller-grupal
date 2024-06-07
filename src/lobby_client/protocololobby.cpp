#include "protocololobby.h"

#include <arpa/inet.h>

#include <QDebug>  // Include the appropriate header for qDebug
#include <iostream>
#include <stdexcept>

void consumir_socket_hasta_linefeed(Socket& socket) {
    char buffer[1] = {0};

    while (buffer[0] != '\n') {
        socket.recvall(buffer, 1);
    }
}

LobbyProtocol::LobbyProtocol(const char* ip, const char* port)
    : socket(ip, port) {}

std::vector<GameMatch> LobbyProtocol::getGameMatches() {
    // TODO: MODIFICAR PROTOCOLO

    // TODO: ENVIAR PETICION PARTIDAS
    char request[] = "Solicito partidas (4 bytes) \nRESPONDER 4 LETRAS: ";
    socket.sendall(request, sizeof(request) - 1);

    char buffer[5] = {0};

    socket.recvall(buffer, 4);

    // Agregado para poder comunicarse con netcat.
    consumir_socket_hasta_linefeed(socket);

    std::string match_name(buffer);
    std::vector<GameMatch> gameMatches;

    for (uint16_t i = 0; i < 5; i++) {
        GameMatch match = {i, 3, 0, match_name + std::to_string(i)};

        gameMatches.push_back(match);
    }

    return gameMatches;
}

void LobbyProtocol::connectToMatch(u_int16_t id) {
    std::string request = "Conectarse a partida " + std::to_string(id) +
                          ". Responder 1 byte \nRESPONDER 'N' PARA SIMULAR "
                          "DEVOLUCION DE ERROR, U OTRO CARACTER PARA EXITO: ";

    socket.sendall((char*)request.c_str(), request.length());

    uint8_t response;
    socket.recvall(&response, 1);

    // Agregado para poder comunicarse con netcat.
    consumir_socket_hasta_linefeed(socket);

    if (response == 'N') {
        throw std::runtime_error("No se pudo conectar a partida");
    }

    qDebug() << "Acá se cerraría esta ventana, y se abriria la ventana de la "
                "partida (el otro ejecutable)";
}

uint16_t LobbyProtocol::createMatch(GameMatch match) {
    std::string request = "Solicito crear partida para " +
                          std::to_string(match.requiredPlayersCount) +
                          " con nombre de partida " + match.name +
                          "\nRESPONDER 'N' PARA SIMULAR DEVOLUCION DE ERROR, U "
                          "OTRO CARACTER PARA EXITO: ";

    socket.sendall((char*)request.c_str(), request.length());

    uint8_t response;
    socket.recvall(&response, 1);

    // Agregado para poder comunicarse con netcat.
    consumir_socket_hasta_linefeed(socket);

    if (response == 'N') {
        throw std::runtime_error("No se pudo crear la partida");
    }

    qDebug() << "Se crea exitosamente la partida. ID: " << match.id << "\n";
    return response;
}
