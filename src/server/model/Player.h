#ifndef PLAYER_H
#define PLAYER_H

#include "../../common/Update.h"
#include "Dynamic_entity.h"
#include "Pickup.h"
#include "constants/pickup_type.h"

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
    enums_value_update::Ammo_type ammo_type;
    int bullets;
public:
    Player(int id, float x_spawn, float y_spawn)
        : Dynamic_entity(id, x_spawn, y_spawn, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_INITIAL_X_VEL, PLAYER_INITIAL_Y_VEL, GRAVITY, true, 0, false, PLAYER_HEALTH),
        points(0), ammo_type(enums_value_update::Ammo_type::NORMAL), bullets(100) {};

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

    std::vector<Update::Update_new> tick(const Map& map,
        std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool) override {
        std::vector<Update::Update_new> updates;

        float old_x = x_pos;
        float old_y = y_pos;

        vel_y += acc_y;

        // validar movimiento en X
        if (vel_x != 0) {
            x_pos += vel_x;

            if (collides_with_map(map)) {
                x_pos -= vel_x;
            }
        }

        // validar movimiento en Y
        if (vel_y != 0) {
            y_pos += vel_y;

            if (collides_with_map(map)) {
                y_pos -= vel_y;
                vel_y = 0;
            }
        }

        if (x_pos != old_x || y_pos != old_y) {
            Update::Update_new update = Update::Update_new::create_position(
                    static_cast<uint16_t>(id),
                    x_pos,
                    y_pos);
            updates.push_back(update);
        }



        // validar contacto con otras entidades
        for (const std::unique_ptr<Dynamic_entity>& other : entity_pool) {
            if (!this->colisiona_con(*other)) {
                continue;
            }

            if (Pickup* pickup = dynamic_cast<Pickup*>(other.get())) {
                Pickup_type type = pickup->getType();
                switch (type) {
                    case COIN:
                        points += pickup->getValue();
                        updates.push_back(Update::Update_new::create_value(
                                static_cast<uint16_t>(id),
                                Update::UpdateType::Score,
                                static_cast<uint8_t>(points)));
                        break;
                    case CARROT:
                        health += pickup->getValue();
                        updates.push_back(Update::Update_new::create_value(
                                static_cast<uint16_t>(id),
                                Update::UpdateType::Health,
                                static_cast<uint8_t>(health)));
                        break;
                    case NORMAL_AMMO:
                        if (ammo_type != enums_value_update::Ammo_type::NORMAL) {
                            ammo_type = enums_value_update::Ammo_type::NORMAL;
                            updates.push_back(Update::Update_new::create_value(
                                    static_cast<uint16_t>(id),
                                    Update::UpdateType::ChangeAmmoType,
                                    enums_value_update::Ammo_type::NORMAL));
                        }
                        bullets += pickup->getValue();
                        updates.push_back(Update::Update_new::create_value(
                                static_cast<uint16_t>(id),
                                Update::UpdateType::BulletsRemaining,
                                static_cast<uint8_t>(bullets)));
                        break;
                    case LIGHT_AMMO:
                        if (ammo_type != enums_value_update::Ammo_type::LIGHT) {
                            ammo_type = enums_value_update::Ammo_type::LIGHT;
                            updates.push_back(Update::Update_new::create_value(
                                    static_cast<uint16_t>(id),
                                    Update::UpdateType::ChangeAmmoType,
                                    enums_value_update::Ammo_type::LIGHT));
                        }
                        bullets += pickup->getValue();
                        updates.push_back(Update::Update_new::create_value(
                                static_cast<uint16_t>(id),
                                Update::UpdateType::BulletsRemaining,
                                static_cast<uint8_t>(bullets)));
                        break;
                    case HEAVY_AMMO:
                        if (ammo_type != enums_value_update::Ammo_type::HEAVY) {
                            ammo_type = enums_value_update::Ammo_type::HEAVY;
                            updates.push_back(Update::Update_new::create_value(
                                    static_cast<uint16_t>(id),
                                    Update::UpdateType::ChangeAmmoType,
                                    enums_value_update::Ammo_type::HEAVY));
                        }
                        bullets += pickup->getValue();
                        updates.push_back(Update::Update_new::create_value(
                                static_cast<uint16_t>(id),
                                Update::UpdateType::BulletsRemaining,
                                static_cast<uint8_t>(bullets)));
                        break;
                    case POWER_AMMO:
                        if (ammo_type != enums_value_update::Ammo_type::POWER) {
                            ammo_type = enums_value_update::Ammo_type::POWER;
                            updates.push_back(Update::Update_new::create_value(
                                    static_cast<uint16_t>(id),
                                    Update::UpdateType::ChangeAmmoType,
                                    enums_value_update::Ammo_type::POWER));
                        }
                        bullets += pickup->getValue();
                        updates.push_back(Update::Update_new::create_value(
                                static_cast<uint16_t>(id),
                                Update::UpdateType::BulletsRemaining,
                                static_cast<uint8_t>(bullets)));
                        break;
                }
                delete_pickup(entity_pool, pickup->get_id());
            }
        }

        return updates;
    }

    void delete_pickup(std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool, int id) {
        auto it = std::find_if(entity_pool.begin(), entity_pool.end(),
                               [id](const std::unique_ptr<Dynamic_entity>& entity) {
                                   return entity->get_id() == id;
                               });
        if (it != entity_pool.end()) {
            entity_pool.erase(it);
        }
    }
};

#endif  // PLAYER_H
