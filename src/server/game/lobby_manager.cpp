#include "lobby_manager.h"

#include <filesystem>

#ifndef MAP_PATH
#define MAP_PATH ""
#endif

// Deberia recibir un socket conectado.
LobbyManager::LobbyManager(Socket socket, GamePoolMonitor& game_pool_monitor)
    : Thread("LobbyManager"),
      protocol(std::move(socket)),
      is_running(true),
      game_pool(game_pool_monitor),
      map_list() {
    // Map new_map = Map::fromYaml("testmap");
    // map_list.push_back(new_map);
    // Map new_map2 = Map::fromYaml("testmap2");
    // map_list.push_back(new_map2);

    // Map new_map3 = Map::fromYaml("map3");
    // map_list.push_back(new_map3);

    for (const auto& map_path :
         std::filesystem::recursive_directory_iterator(MAP_PATH)) {
            std::string map_name = map_path.path().stem().string();
            Map new_map = Map::fromYaml(map_name);
            map_list.push_back(new_map);
        }
}

uint8_t LobbyManager::create_game(GameMatch& match_info) {
    auto map_it = std::find_if(
        map_list.begin(), map_list.end(),
        [&](const Map& map) { return map.get_name() == match_info.map; });

    if (map_it == map_list.end()) {
        throw std::runtime_error("No se encontró el mapa.");
    }

    match_id new_match_id = game_pool.emplace_game(match_info.name, *map_it);

    return new_match_id;
}

std::pair<uint16_t, std::string> LobbyManager::join_game(uint8_t game_id) {
    uint16_t player_id;
    std::string map_name = "";

    player_id = game_pool.add_player(game_id);
    map_name = game_pool.get_map_name(game_id);

    return std::make_pair(player_id, map_name);
}

void LobbyManager::sendSocketOfClientToGame(uint8_t game_id, uint16_t player_id,
                                            LobbyProtocol& client) {
    Socket socket_client = protocol.extract_socket();

    game_pool.add_socket_for_player(game_id, player_id,
                                    std::move(socket_client));
}

void LobbyManager::process_command(MessageType command) {
    switch (command) {
        case CREATE: {
            GameMatch match_info;
            match_info = protocol.receive_create_match();
            uint8_t created_game_id;
            created_game_id = create_game(match_info);
            protocol.send_created_match(created_game_id);
            break;
        }
        case JOIN: {
            uint8_t game_id = protocol.receive_join_game();
            std::pair<uint16_t, std::string> joined_info = join_game(game_id);
            protocol.send_join_game(joined_info);
            sendSocketOfClientToGame(game_id, joined_info.first, protocol);
            is_running = false;
            break;
        }
        case GET_GAMES: {
            std::vector<GameMatch> games = game_pool.get_games();
            protocol.send_games(games);
            break;
        }
        default:
            break;
    }
}

void LobbyManager::run() {
    while (is_running) {
        try {
            MessageType command = protocol.receive_command();

            process_command(command);
        } catch (const ClosedConnectionError& e) {
            // En el caso de que se cierre la conexion, simplemente dejo de
            // ejecutar el hilo. Eventualmente ClientListener joineará este
            // hilo.
            is_running = false;
        }
    }
}

void LobbyManager::kill() { is_running = false; }
