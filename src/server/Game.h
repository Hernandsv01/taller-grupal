#ifndef GAME_H
#define GAME_H

#include "common/library/thread.h"
#include "common/dtos.h"
#include "constants/game_status.h"
#include "Entity.h"
#include "Dynamic_entity.h"
#include "clients/client.h"
#include "clients/client_monitor.h"
#include <vector>
#include <chrono>

#define TPS 24
#define SECOND_IN_NANO 1000000000
#define TICK_DURATION (std::chrono::nanoseconds{(SECOND_IN_NANO / TPS)})


class Game : public Thread{
private:
    std::chrono::steady_clock reloj;

    Game_status status;
    std::vector<Dynamic_entity> entity_pool;
public:
    explicit Game();
    void run() override;
    void run_iteration();
    void process_action(uint8_t action, int player);
};


#endif //GAME_H
