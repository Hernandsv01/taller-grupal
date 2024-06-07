#include "protocololobby.h"

#include <arpa/inet.h>

#include <iostream>
#include <stdexcept>

LobbyProtocol::LobbyProtocol(const char* ip, const char* port)
    : socket(ip, port) {}

std::vector<GameMatch> LobbyProtocol::getGameMatches() {
    // TODO: MODIFICAR PROTOCOLO

    // TODO: ENVIAR PETICION PARTIDAS
    char request[] = "Solicito partidas (4 bytes)";
    socket.sendall(request, sizeof(request) - 1);

    char buffer[5] = {0};

    socket.recvall(buffer, 4);

    std::string match_name(buffer);
    std::vector<GameMatch> gameMatches;

    for (uint16_t i = 0; i < 5; i++) {
        GameMatch match = {i, 3, 0, match_name + std::to_string(i)};

        gameMatches.push_back(match);
    }

    return gameMatches;
}

void LobbyProtocol::connectToMatch(u_int16_t id) {
    std::string request =
        "Conectarse a partida " + std::to_string(id) + ". Responder 1 byte";

    socket.sendall((char*)request.c_str(), request.length());

    uint8_t response;
    socket.recvall(&response, 1);

    if (response == 0) {
        throw std::runtime_error("No se pudo conectar a partida");
    }
}

uint16_t LobbyProtocol::createMatch(GameMatch match) {
    std::string request = "Solicito crear partida para " +
                          std::to_string(match.requiredPlayersCount) +
                          " con nombre de partida " + match.name;

    socket.sendall((char*)request.c_str(), request.length());

    uint16_t response;
    socket.recvall(&response, 2);

    response = ntohs(response);

    if (response == 0) {
        throw std::runtime_error("No se pudo crear la partida");
    }

    std::cout << response << std::endl;
    return response;
}
