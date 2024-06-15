#ifndef PLAYER_H
#define PLAYER_H

#include "../../common/Update.h"
#include "Dynamic_entity.h"
#include "updateables.h"
#include <cmath>

#define PLAYER_HEIGHT 5
#define PLAYER_WIDTH 5
#define PLAYER_HEALTH 100

class Player : public Dynamic_entity {
public:
    Player(int id, float x_spawn, float y_spawn)
        : Dynamic_entity(id, x_spawn, y_spawn, PLAYER_WIDTH, PLAYER_HEIGHT, 0, 0, 5, true, 0, false, PLAYER_HEALTH) {};

    std::vector<Update> tick(Map map,
        std::vector<std::unique_ptr<Dynamic_entity>>* entity_pool) override {
        std::vector<Update> updates;

//        vel_x += acc_x;
        vel_y += acc_y;

        // validar movimiento en X
        if (vel_x != 0) {
            x_pos += vel_x;

            bool collides = false;

            for (float i = this->x_min(); i < this->x_max() && !collides; i++) {
                for (float j = this->y_min(); j < this->y_max() && !collides; j++) {
                    Collision collision = map.get_block_collision({static_cast<uint8_t>(std::floor(i)),static_cast<uint8_t>(std::floor(j))});
                    if (collision != Collision::Air) {
                        collides = true;
                    }
                }
            }

            if (!collides) {
                Update update{static_cast<uint16_t>(id),
                              Updateables::POSITION_X,
                              static_cast<uint32_t>(x_pos)};
                updates.push_back(update);
            } else {
                x_pos -= vel_x;
            }
        }

        // validar movimiento en Y
        if (vel_y != 0) {
            y_pos += vel_y;

            bool collides = false;

            for (float i = this->x_min(); i < this->x_max() && !collides; i++) {
                for (float j = this->y_min(); j < this->y_max() && !collides; j++) {
                    Collision collision = map.get_block_collision({static_cast<uint8_t>(std::floor(i)),static_cast<uint8_t>(std::floor(j))});
                    if (collision != Collision::Air) {
                        collides = true;
                    }
                }
            }

            if (!collides) {
                Update update{static_cast<uint16_t>(id),
                              Updateables::POSITION_Y,
                              static_cast<uint32_t>(y_pos)};
                updates.push_back(update);
            } else {
                y_pos -= vel_y;
                vel_y = 0;
            }
        }

        // validar contacto con otras entidades
        for (const std::unique_ptr<Dynamic_entity>& other : *entity_pool) {
            if (!this->colisiona_con(*other)) {
                continue;
            }

            // procesamiento de colision con objetos que hacen daño fue movido a los objetos dañinos en sí
            // no se procesa del lado del receptor del daño

            // falta chequeo de items
        }

        return updates;
    }
};

#endif  // PLAYER_H
