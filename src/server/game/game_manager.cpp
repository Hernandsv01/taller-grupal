#include "game_manager.h"

GameManager::GameManager(Socket& socket) : Thread("Game manager"), protocol(socket), is_running(true) {}

uint16_t GameManager::create_game(GameMatch& match_info) {
    uint16_t new_match_id = 00;
    //se deberia crear el game y setear los ids
    return new_match_id;
}

void GameManager::join_game() {
    //se deberia agregar el
}


void GameManager::process_command(MessageType& command) {
    switch (command) {
        case CREATE: {
                    //tiene que recibir
                    GameMatch match_info;
                    match_info = protocol.receive_create_match();
                    uint16_t created_game_id;
                    created_game_id = create_game(match_info);
                    protocol.send_created_match(created_game_id);
                    break;
        }
        case JOIN:{
            protocol.receive_join_game();
            //acciona join game
            uint16_t assigned_id;
            protocol.send_join_game(assigned_id);
            break;
        }
        case GET_GAMES: {
            protocol.get_games(game_pool);
            break;
        }
        default:
            break;
    }
}

void GameManager::run() {
    //ver cual es la condicion de corte de este hilo.
    while(true){
        MessageType command = protocol.receive_command();
        process_command(command);
    }

}



