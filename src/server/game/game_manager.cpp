#include "game_manager.h"

GameManager::GameManager(Socket& socket) : Thread("Game manager"), protocol(socket), is_running(true) {
    try{
        Map new_map = Map::fromYaml("../../common/map/testmap.yaml");
        map_list.push_back(new_map);
        Map new_map2 = Map::fromYaml("../../common/map/testmap2.yaml");
        map_list.push_back(new_map2);
    } catch (std::exception& e) {
        std::cerr << "Failed to load maps: " << e.what() << std::endl;
    }
}



uint8_t GameManager::create_game(GameMatch& match_info) {
    uint16_t new_match_id = game_pool.size();
    for(Map map : map_list){
        if (map.get_name() == match_info.map){
            game_pool.emplace_back(std::make_unique<Game>(match_info.name,map));
            game_pool[(game_pool.size()-1)]->set_id(new_match_id);
        }
    }
    return new_match_id;
}

uint16_t GameManager::join_game(uint8_t game_id) {
    uint16_t player_id;
    for(const auto& game: game_pool){
        if(game->get_id() == game_id){
            player_id = game->add_player();
        }
    }
    return player_id;
}


void GameManager::process_command(MessageType& command) {
    switch (command) {
        case CREATE: {
            GameMatch match_info;
            match_info = protocol.receive_create_match();
            uint8_t created_game_id;
            created_game_id = create_game(match_info);
            protocol.send_created_match(created_game_id);
            break;
        }
        case JOIN:{
            uint8_t game_id = protocol.receive_join_game();
            uint16_t assigned_id = join_game(game_id);
            protocol.send_join_game(assigned_id);
            //FALTA ENVIAR INFO MAPA BACK
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
    while(is_running){
        MessageType command = protocol.receive_command();
        process_command(command);
        //flush de juegos terminados en la pool de games
    }
}

void GameManager::kill() {
    is_running = false;
}

