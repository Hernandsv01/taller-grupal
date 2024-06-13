#include "protocololobby.h"

#include <arpa/inet.h>

#include <QDebug>  // Include the appropriate header for qDebug
#include <cstring>
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

        char buffer_match_name[17] = {0};
        socket.recvall(buffer_match_name, sizeof(buffer_match_name) - 1);

        std::string match_name(buffer_match_name);

        char buffer_map_name[33] = {0};
        socket.recvall(buffer_map_name, sizeof(buffer_map_name) - 1);

        std::string map_name(buffer_map_name);

        games[i] = GameMatch{id, match_name, map_name};
    }

    return games;
}

std::pair<uint16_t, std::string> LobbyProtocol::joinMatch(match_id id) {
    MessageType type = JOIN;
    socket.sendall(&type, sizeof(type));

    socket.sendall(&id, sizeof(id));

    // Recibir id jugador
    uint16_t player_id;
    socket.recvall(&player_id, sizeof(player_id));
    player_id = ntohs(player_id);

    // Recibir mapa (CUANDO DECIDAMOS)

    char buffer_map_name[33] = {0};
    socket.recvall(buffer_map_name, sizeof(buffer_map_name) - 1);

    std::string map_name(buffer_map_name);

    return std::make_pair(player_id, map_name);
}

match_id LobbyProtocol::createMatch(const std::string& match_name,
                                    const std::string& map_name) {
    if (match_name.length() > 16) {
        throw std::runtime_error(
            "El nombre de la partida es demasiado largo. El máximo es de "
            "16 caracteres.");
    }

    MessageType type = CREATE;
    socket.sendall(&type, sizeof(type));

    uint8_t buffer_match_name[16] = {0};
    memcpy(buffer_match_name, match_name.c_str(), match_name.length());
    socket.sendall(buffer_match_name, sizeof(buffer_match_name));

    uint8_t buffer_map_name[32] = {0};
    memcpy(buffer_map_name, map_name.c_str(), map_name.length());
    socket.sendall(buffer_map_name, sizeof(buffer_map_name));

    match_id id;
    socket.recvall(&id, sizeof(id));

    return id;
}
