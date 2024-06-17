#ifndef PLAYER_H
#define PLAYER_H

#include "../../common/Update.h"
#include "Dynamic_entity.h"
#include "updateables.h"
#include "Pickup.h"
#include "constants/pickup_type.h"
#include "constants/ammo_type.h"

#include <cmath>

#define PLAYER_HEIGHT 5
#define PLAYER_WIDTH 5
#define PLAYER_HEALTH 100
#define PLAYER_INITIAL_X_VEL 0
#define PLAYER_INITIAL_Y_VEL 0
#define GRAVITY 5

class Player : public Dynamic_entity {
private:
    int points;
    Ammo_type ammo_type;
    int bullets;
public:
    Player(int id, float x_spawn, float y_spawn)
        : Dynamic_entity(id, x_spawn, y_spawn, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_INITIAL_X_VEL, PLAYER_INITIAL_Y_VEL, GRAVITY, true, 0, false, PLAYER_HEALTH) {};

    void process_action(uint8_t action) {
        switch (action) {
            case JUMP:
                setYSpeed(10);
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

            // TODO: falta eliminación de items al ser agarrados
            if (auto pickup = std::dynamic_pointer_cast<Pickup>(other)) {
                Pickup_type type = pickup->getType();
                switch (type) {
                    case POINTS:
                        points += pickup.getValue();
                        Update update{static_cast<uint16_t>(id),
                                      Updateables::POINTS,
                                      static_cast<uint32_t>(points)};
                        updates.push_back(update);
                        break;
                    case HEALTH:
                        health += pickup.getValue();
                        Update update{static_cast<uint16_t>(id),
                                      Updateables::HEALTH,
                                      static_cast<uint32_t>(health)};
                        updates.push_back(update);
                        break;
                    case NORMAL_AMMO:
                        if (ammo_type != Ammo_type::NORMAL) {
                            ammo_type = Ammo_type::NORMAL;
                            Update update{static_cast<uint16_t>(id),
                                          Updateables::AMMO_TYPE,
                                          static_cast<uint32_t>(Ammo_type::NORMAL)};
                            updates.push_back(update);
                        }
                        bullets += pickup.getValue();
                        Update update{static_cast<uint16_t>(id),
                                      Updateables::BULLETS,
                                      static_cast<uint32_t>(bullets)};
                        updates.push_back(update);
                        break;
                    case LIGHT_AMMO:
                        if (ammo_type != Ammo_type::LIGHT) {
                            ammo_type = Ammo_type::LIGHT;
                            Update update{static_cast<uint16_t>(id),
                                          Updateables::AMMO_TYPE,
                                          static_cast<uint32_t>(Ammo_type::LIGHT)};
                            updates.push_back(update);
                        }
                        bullets += pickup.getValue();
                        Update update{static_cast<uint16_t>(id),
                                      Updateables::BULLETS,
                                      static_cast<uint32_t>(bullets)};
                        updates.push_back(update);
                        break;
                    case HEAVY_AMMO:
                        if (ammo_type != Ammo_type::HEAVY) {
                            ammo_type = Ammo_type::HEAVY;
                            Update update{static_cast<uint16_t>(id),
                                          Updateables::AMMO_TYPE,
                                          static_cast<uint32_t>(Ammo_type::HEAVY)};
                            updates.push_back(update);
                        }
                        bullets += pickup.getValue();
                        Update update{static_cast<uint16_t>(id),
                                      Updateables::BULLETS,
                                      static_cast<uint32_t>(bullets)};
                        updates.push_back(update);
                        break;
                    case POWER_AMMO:
                        if (ammo_type != Ammo_type::POWER) {
                            ammo_type = Ammo_type::POWER;
                            Update update{static_cast<uint16_t>(id),
                                          Updateables::AMMO_TYPE,
                                          static_cast<uint32_t>(Ammo_type::POWER)};
                            updates.push_back(update);
                        }
                        bullets += pickup.getValue();
                        Update update{static_cast<uint16_t>(id),
                                      Updateables::BULLETS,
                                      static_cast<uint32_t>(bullets)};
                        updates.push_back(update);
                        break;
                }
            }
        }

        return updates;
    }
};

#endif  // PLAYER_H
