#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <vector>

#include "../../common/Update.h"
#include "../../common/library/thread.h"
#include "clients/client_monitor.h"
#include "model/Dynamic_entity.h"
#include "model/Player.h"
#include "model/constants/game_status.h"
#include "server_client.h"

#define TPS 24
#define SECOND_IN_NANO 1000000000
#define TICK_DURATION (std::chrono::nanoseconds{(SECOND_IN_NANO / TPS)})

class Game : public Thread {
   private:
    Game_status status;
    Map map;
    std::vector<std::unique_ptr<Dynamic_entity>> entity_pool;

   public:
    explicit Game(Map map);
    void run() override;
    void run_iteration();
    void stop_custom() override;
};

#endif  // GAME_H
