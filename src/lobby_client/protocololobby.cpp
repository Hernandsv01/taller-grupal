#include "protocololobby.h"

#include <arpa/inet.h>

#include <QDebug>  // Include the appropriate header for qDebug
#include <iostream>
#include <stdexcept>

LobbyProtocol::LobbyProtocol(const char* ip, const char* port)
    : socket(ip, port) {}

std::vector<GameMatch> LobbyProtocol::getGameMatches() {
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

uint16_t LobbyProtocol::joinMatch(match_id id) {
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

match_id LobbyProtocol::createMatch(const std::string& match_name) {
    if (match_name.length() > 16) {
        throw std::runtime_error(
            "El nombre de la partida es demasiado largo. El máximo es de "
            "16 caracteres.");
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
