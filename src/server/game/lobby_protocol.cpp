#include "lobby_protocol.h"

//ver tema de manejo del socket.
LobbyProtocol::LobbyProtocol(Socket& socket) : socket(socket) {}


MessageType LobbyProtocol::receive_command() {
    MessageType command;
    socket.recvall(&command, sizeof(command));
    return command;
}

GameMatch LobbyProtocol::receive_create_match() {
    GameMatch match_data;
    //nombre_partida (sizeof(char)*16) (no es necesario enviar nullbyte)
    //Mapa (probablemente sizeof(char)*32 (no es necesario enviar nullbyte))
    socket.recvall(&match_data.name, sizeof(char)*16);
    socket.recvall(&match_data.map, sizeof(char)*32);
    return match_data;
}

void LobbyProtocol::send_created_match(uint8_t& match_id) {
    socket.sendall(&match_id, sizeof(uint16_t));
}

match_id LobbyProtocol::receive_join_game() {
    match_id id;
    socket.recvall(&id, sizeof(match_id));
    return id;
}

void LobbyProtocol::send_join_game(std::pair<uint16_t, std::string> &joined_info) { //falta mapa
    uint16_t player_id= htons(joined_info.first);
    socket.sendall(&player_id, sizeof(uint16_t));
}

void LobbyProtocol::get_games(std::vector<std::unique_ptr<Game>> &games) {
    //tengo que usar solo la info de games que importa
    //envia cantidad de partidas uint8_t
    //envia el vector { //id_partida (uint8_t)
                        //nombre_partida (sizeof(char)*16) (no es necesario enviar nullbyte)
                        //Mapa (probablemente sizeof(char)*32 (no es necesario enviar nullbyte
                        // }
}


