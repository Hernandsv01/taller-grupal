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
    auto map_it = std::find_if(map_list.begin(), map_list.end(), [&](const Map& map) {
        return map.get_name() == match_info.map;
    });

    if (map_it != map_list.end()) {
        game_pool.emplace_back(std::make_unique<Game>(match_info.name, *map_it));
        game_pool.back()->set_id(new_match_id);
    }

    return new_match_id;
}

std::pair<uint16_t, std::string> GameManager::join_game(uint8_t game_id) {
    uint16_t player_id;
    std::string map_name = "";
    for(const auto& game: game_pool){
        if(game->get_id() == game_id){
            player_id = game->add_player();
            map_name = game->get_map_name();
        }
    }

    return std::make_pair(player_id, map_name);
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
            std::pair<uint16_t, std::string> joined_info = join_game(game_id);
            protocol.send_join_game(joined_info);
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
    //end all games running
}



