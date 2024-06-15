#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <vector>

#include "../common/Update.h"
#include "../common/library/thread.h"
#include "clients/client_monitor.h"
#include "model/Dynamic_entity.h"
#include "model/Player.h"
#include "model/constants/game_status.h"
#include "clients/server_client.h"
#include "../common/ActionType.h"
#include "../common/map/map.h"


#define TPS 24
#define SECOND_IN_NANO 1000000000
#define TICK_DURATION (std::chrono::nanoseconds{(SECOND_IN_NANO / TPS)})

#include "../common/library/thread.h"

class Game : public Thread {
   private:
    std::chrono::steady_clock reloj;
    uint8_t id;
    Game_status status;
    Map map;
    std::string name;

    std::vector<std::unique_ptr<Dynamic_entity>> entity_pool;

   public:
    explicit Game(std::string name, Map map);
    void run() override;
    void run_iteration();
    void process_action(uint8_t action, int player);
    void stop_custom() override;
    uint16_t add_player();
    uint8_t get_id();
    void set_id(uint8_t id);
    std::string get_match_name();
    std::string get_map_name();
};

#endif  // GAME_H
