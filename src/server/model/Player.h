#ifndef PLAYER_H
#define PLAYER_H

#include "../../common/Update.h"
#include "Dynamic_entity.h"
#include "updateables.h"
#include <cmath>

#define PLAYER_HEIGHT 5
#define PLAYER_WIDTH 5
#define PLAYER_HEALTH 100
#define PLAYER_INITIAL_X_VEL 0
#define PLAYER_INITIAL_Y_VEL 0
#define GRAVITY 5

class Player : public Dynamic_entity {
public:
    Player(int id, float x_spawn, float y_spawn)
        : Dynamic_entity(id, x_spawn, y_spawn, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_INITIAL_X_VEL, PLAYER_INITIAL_Y_VEL, GRAVITY, true, 0, false, PLAYER_HEALTH) {};

    void process_action(uint8_t action) {
        switch (action) {
            case JUMP:
                // Not implemented: JUMP action
                return;

            case RUN_LEFT:
                setXSpeed(-3);
                break;

            case RUN_RIGHT:
                setXSpeed(3);
                break;

            case SHOOT_RIGHT:
                // Not implemented: SHOOT_RIGHT action
                return;

            case SHOOT_LEFT:
                // Not implemented: SHOOT_LEFT action
                return;

            case SHOOT_UP:
                // Not implemented: SHOOT_UP action
                return;

            case SHOOT_DOWN:
                // Not implemented: SHOOT_DOWN action
                return;

            case SPECIAL:
                // Not implemented: SPECIAL action
                return;

            case STOP_RUN_RIGHT:
                if (vel_x > 0) {
                    vel_x = 0;
                }
                break;

            case STOP_RUN_LEFT:
                if (vel_x < 0) {
                    vel_x = 0;
                }
                break;

            default:
                // Handle unexpected actions if necessary
                break;
        }
    };

    std::vector<Update> tick(const Map& map,
        std::vector<std::unique_ptr<Dynamic_entity>>* entity_pool) override {
        std::vector<Update> updates;

        vel_y += acc_y;

        // validar movimiento en X
        if (vel_x != 0) {
            x_pos += vel_x;

            if (collides_with_map(map)) {
                x_pos -= vel_x;
            } else {
                Update update{static_cast<uint16_t>(id),
                              Updateables::POSITION_X,
                              static_cast<uint32_t>(x_pos)};
                updates.push_back(update);
            }
        }

        // validar movimiento en Y
        if (vel_y != 0) {
            y_pos += vel_y;

            if (collides_with_map(map)) {
                y_pos -= vel_y;
                vel_y = 0;
            } else {
                Update update{static_cast<uint16_t>(id),
                              Updateables::POSITION_Y,
                              static_cast<uint32_t>(y_pos)};
                updates.push_back(update);
            }
        }

        // validar contacto con otras entidades
        for (const std::unique_ptr<Dynamic_entity>& other : *entity_pool) {
            if (!this->colisiona_con(*other)) {
                continue;
            }

            // procesamiento de colision con objetos que hacen daño fue movido a los objetos dañinos en sí
            // no se procesa del lado del receptor del daño

            // TODO: falta chequeo de items
        }

        return updates;
    }
};

#endif  // PLAYER_H
