#ifndef GAME_H
#define GAME_H

#include "common/library/thread.h"
#include "common/dtos.h"
#include "constants/game_status.h"
#include "Entity.h"
#include "Dynamic_entity.h"
#include "clients/client.h"
#include "clients/client_monitor.h"

class Game : public Thread{
private:
    Game_status status;
    std::vector<Dynamic_entity> entity_pool;
public:
    explicit Game();
    void run() override;
    void process_action(uint8_t action, int player);
};


#endif //GAME_H
