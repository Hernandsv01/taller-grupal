#ifndef PLAYER_H
#define PLAYER_H

#include "../../common/Update.h"
#include "Dynamic_entity.h"
#include "updateables.h"

class Player : public Dynamic_entity {
   public:
    Player(int id, int x_spawn, int y_spawn)
        : Dynamic_entity(id, x_spawn, y_spawn, 5, 5, true, 0, 0, 0){};
    std::vector<Update::Update_new> tick(
        std::vector<std::unique_ptr<Dynamic_entity>> *entity_pool) override {
        std::vector<Update::Update_new> updates;

        int old_x = pos_x;
        int old_y = pos_y;

        if (vel_x != 0) {
            pos_x += vel_x;
            // falta verificar colisión antes de agregar el update
        }
        if (vel_y != 0) {
            pos_y += vel_y;
            // falta verificar colisión antes de agregar el update
        }

        bool cambio_posicion = (old_x != pos_x || old_y != pos_y);

        if (cambio_posicion) {
            Update::Update_new update = Update::Update_new::create_position(
                id, (float)pos_x, (float)pos_y);
            updates.push_back(update);
        }
        return updates;
    }
};

#endif  // PLAYER_H
