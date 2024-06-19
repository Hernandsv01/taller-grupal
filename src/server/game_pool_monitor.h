#ifndef GAME_POOL_MONITOR_H
#define GAME_POOL_MONITOR_H

#include <mutex>
#include <vector>

#include "../common/lobbyprotocol.h"
#include "Game.h"

class GamePoolMonitor {
    std::set<uint8_t> avaible_ids;
    std::mutex avaible_ids_mutex;

    std::map<uint8_t, std::unique_ptr<Game>> games;
    // std::map<uint8_t, std::mutex> mutexes_for_games;
    std::mutex mutex_vector;

    uint8_t extract_avaible_id() {
        std::lock_guard<std::mutex> lock(avaible_ids_mutex);
        uint8_t id = *avaible_ids.begin();
        avaible_ids.erase(id);
        return id;
    }

    void return_id(uint8_t id) {
        std::lock_guard<std::mutex> lock(avaible_ids_mutex);
        avaible_ids.insert(id);
    }

   public:
    GamePoolMonitor() {
        for (uint8_t i = 0; i < 255; i++) {
            avaible_ids.insert(i);
        }
    };

    GamePoolMonitor(const GamePoolMonitor&) = delete;
    GamePoolMonitor& operator=(const GamePoolMonitor&) = delete;

    // Le asigna id al juego
    match_id emplace_game(std::string name, Map map) {
        std::lock_guard<std::mutex> lock(mutex_vector);
        match_id id = extract_avaible_id();

        games.insert({id, std::make_unique<Game>(name, map)});
        games[id]->set_id(id);

        return id;
    }

    void remove_game(uint8_t game_id) {
        std::lock_guard<std::mutex> lock(mutex_vector);
        games.erase(game_id);
        return_id(game_id);
    }

    std::string get_map_name(uint8_t game_id) {
        std::lock_guard<std::mutex> lock(mutex_vector);
        return games.at(game_id)->get_map_name();
    }

    uint16_t add_player(uint8_t game_id) {
        std::lock_guard<std::mutex> lock(mutex_vector);
        return games.at(game_id)->add_player();
    }

    void add_socket_for_player(uint8_t game_id, uint16_t player_id,
                               Socket&& socket) {
        std::lock_guard<std::mutex> lock(mutex_vector);
        games.at(game_id)->add_socket_for_player(player_id, std::move(socket));
    }

    std::vector<GameMatch> get_games() {
        std::lock_guard<std::mutex> lock(mutex_vector);
        std::vector<GameMatch> games_info;
        for (auto& [_, game] : games) {
            games_info.push_back(GameMatch{
                game->get_id(), game->get_match_name(), game->get_map_name()});
        }
        return games_info;
    }

    ~GamePoolMonitor() {
        // En realidad no sería necesario porque los thread ya son RAII, pero
        // bueno.
        // Así me queda la mente más tranquila.
        for (auto& [_, game] : games) {
            game->stop();
            game->join();
        }
    }
};

#endif
