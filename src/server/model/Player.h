#ifndef PLAYER_H
#define PLAYER_H

#include "../../common/Update.h"
#include "Dynamic_entity.h"
#include "updateables.h"

class Player : public Dynamic_entity {
   public:
    Player(int id, int x_spawn, int y_spawn)
        : Dynamic_entity(id, x_spawn, y_spawn, 5, 5, true, 0, 0, 0){};
    std::vector<Update> tick(
        std::vector<Dynamic_entity>* entity_pool) override {
        std::vector<Update> updates;
        if (vel_x != 0) {
            pos_x += vel_x;

            // falta verificar colisión

            Update update{static_cast<uint8_t>(id), Updateables::POSITION_X,
                          static_cast<uint8_t>(pos_x)};
            updates.push_back(update);
        }
        if (vel_y != 0) {
            pos_y += vel_y;

            // falta verificar colisión

            Update update{static_cast<uint8_t>(id), Updateables::POSITION_Y,
                          static_cast<uint8_t>(pos_y)};
            updates.push_back(update);
        }
        return updates;
    }
};

#endif  // PLAYER_H
