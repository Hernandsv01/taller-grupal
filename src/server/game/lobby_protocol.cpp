#include "lobby_protocol.h"


LobbyProtocol::LobbyProtocol(Socket& socket) : socket(socket) {}


MessageType LobbyProtocol::receive_command() {
    MessageType command;
    socket.recvall(&command, sizeof(command));
    return command;
}

GameMatch LobbyProtocol::receive_create_match() {
    GameMatch match_data;
    char buffer_name[17] = {0};
    socket.recvall(buffer_name, sizeof(buffer_name) - 1);
    char buffer_map_name[33] = {0};
    socket.recvall(buffer_map_name, sizeof(buffer_map_name) - 1);
    match_data.name = buffer_name;
    match_data.map = buffer_map_name;
    return match_data;
}

void LobbyProtocol::send_created_match(match_id& id) {
    socket.sendall(&id, sizeof(match_id));
}

match_id LobbyProtocol::receive_join_game() {
    match_id id;
    socket.recvall(&id, sizeof(match_id));
    return id;
}

void LobbyProtocol::send_join_game(std::pair<uint16_t, std::string> &joined_info) {
    uint16_t player_id= htons(joined_info.first);
    socket.sendall(&player_id, sizeof(uint16_t));
    uint8_t buffer_map_name[32] = {0};
    memcpy(buffer_map_name, joined_info.second.c_str(), joined_info.second.length());
    socket.sendall(buffer_map_name, sizeof(buffer_map_name));
}

void LobbyProtocol::get_games(std::vector<std::unique_ptr<Game>> &games) {
    uint8_t games_count = games.size();
    socket.sendall(&games_count, sizeof(uint8_t));

    for (const auto &game : games) {
        match_id id = game->get_id();
        socket.sendall(&id, sizeof(id));

        std::string match_name = game->get_match_name();
        char buffer_match_name[16] = {0};
        memcpy(buffer_match_name, match_name.c_str(), match_name.length());
        socket.sendall(buffer_match_name, sizeof(buffer_match_name));

        std::string map_name = game->get_map_name();
        char buffer_map_name[32] = {0};
        strncpy(buffer_map_name, map_name.c_str(), map_name.length());
        socket.sendall(buffer_map_name, sizeof(buffer_map_name));

        //otra forma si envio un GameMatch//GameInfoDTO por iteracion
//        GameMatch match_info;
//        match_info.id = game->get_id();
//        match_info.name = game->get_match_name();
//        match_info.map = game->get_map_name();
//        socket.sendall(&match_info, sizeof(GameMatch));
    }

}


