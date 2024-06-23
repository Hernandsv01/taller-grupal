#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <vector>

#include "../common/ActionType.h"
#include "../common/Update.h"
#include "../common/library/thread.h"
#include "../common/map/map.h"
#include "clients/server_client.h"
#include "model/Dynamic_entity.h"
#include "model/Player.h"
#include "model/Enemy.h"
#include "model/constants/game_status.h"

#define TPS 24
#define SECOND_IN_NANO 1000000000
#define TICK_DURATION (std::chrono::nanoseconds{(SECOND_IN_NANO / TPS)})

#include "../common/library/thread.h"

class Game : public Thread {
   private:
    uint8_t id;
    Game_status status;
    Map map;
    std::string name;
    int next_id;

    std::vector<std::unique_ptr<Dynamic_entity>> entity_pool;
    std::vector<std::unique_ptr<Server_Client>> clients;

   public:
    explicit Game(std::string name, Map map);
    void run() override;
    void run_iteration();
    void initialize_values();
    void stop_custom() override;
    uint16_t add_player();
    void add_socket_for_player(uint16_t player_id, Socket socket);
    void sendAll(std::vector<Update::Update_new> updates);
    int findEntityPositionById(int id);
    uint8_t get_id();
    void set_id(uint8_t id);
    std::string get_match_name();
    std::string get_map_name();
    std::vector<Update::Update_new> get_full_game_updates();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
};

#endif  // GAME_H
