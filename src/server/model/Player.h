#ifndef PLAYER_H
#define PLAYER_H

#include "../../common/Update.h"
#include "Dynamic_entity.h"
#include "updateables.h"

class Player : public Dynamic_entity {
private:
    int health;
public:
    Player(int id, int x_spawn, int y_spawn)
        : Dynamic_entity(id, x_spawn, y_spawn, 5, 5, 0, 0, true, 0, false), health(100) {};

    std::vector<Update> tick(
        std::vector<std::unique_ptr<Dynamic_entity>>* entity_pool) override {
        std::vector<Update> updates;

//        vel_x += acc_x;
//        vel_y += acc_y;

        if (vel_x != 0) {
            x_pos += vel_x;

            // falta verificar colisión con bloques de mapa antes de agregar el update

            Update update{static_cast<uint16_t>(id),
                          Updateables::POSITION_X,
                          static_cast<uint32_t>(x_pos)};
            updates.push_back(update);
        }

        if (vel_y != 0) {
            y_pos += vel_y;

            // falta verificar colisión con bloques de mapa antes de agregar el update

            Update update{static_cast<uint16_t>(id),
                          Updateables::POSITION_Y,
                          static_cast<uint32_t>(y_pos)};
            updates.push_back(update);
        }

        for (const std::unique_ptr<Dynamic_entity>& other : *entity_pool) {
            if (!this->colisiona_con(*other)) {
                continue;
            }

            if (this->is_damageable && other->get_damage_dealt() != 0) {
                this->health -= other->get_damage_dealt();
                Update update{static_cast<uint16_t>(id),
                              Updateables::HEALTH,
                              static_cast<uint32_t>(health)};
                updates.push_back(update);
                // falta verificar si objeto que hizo daño debe destruirse (caso bala)
            }

            // falta chequeo de items
        }

        return updates;
    }
};

#endif  // PLAYER_H
