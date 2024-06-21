#ifndef PLAYER_H
#define PLAYER_H

#include "../../common/Update.h"
#include "Dynamic_entity.h"
#include "Pickup.h"
#include "Bullet.h"
#include "constants/pickup_type.h"

#include <cmath>
#include <map>

#define PLAYER_HEIGHT 5
#define PLAYER_WIDTH 5
#define PLAYER_HEALTH 100
#define PLAYER_INITIAL_X_VEL 0
#define PLAYER_INITIAL_Y_VEL 0
#define GRAVITY 5
#define SECONDS_UNTIL_RESPAWN 3
#define SECONDS_IMMUNE_AFTER_DAMAGE 2

class Player : public Dynamic_entity {
private:
    int points;
    enums_value_update::Ammo_type ammo_type;
    std::map<enums_value_update::Ammo_type, int> ammo;
public:
    Player(int id, float x_spawn, float y_spawn)
        : Dynamic_entity(id, x_spawn, y_spawn, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_INITIAL_X_VEL, PLAYER_INITIAL_Y_VEL, GRAVITY, true, 0, false, PLAYER_HEALTH, true, true),
        points(0), ammo_type(enums_value_update::Ammo_type::NORMAL) {
        ammo[enums_value_update::Ammo_type::LIGHT] = 0;
        ammo[enums_value_update::Ammo_type::HEAVY] = 0;
        ammo[enums_value_update::Ammo_type::POWER] = 0;
    };

    std::vector<Update::Update_new> process_action(uint8_t action, std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool, int& next_id) {
        std::vector<Update::Update_new> updates;
        switch (action) {
            case JUMP:
                setYSpeed(10);
                break;
            case RUN_LEFT:
                setXSpeed(-3);
                break;

            case RUN_RIGHT:
                setXSpeed(3);
                break;

            case SHOOT:
                if (ammo[ammo_type] <= 0) {
                    return updates;
                }
                float x_spawn;
                float y_spawn;
                float speed;

                if (looking_right) {
                    x_spawn = x_pos+x_size;
                    y_spawn = y_pos+(y_size/2);
                    speed = 5;
                } else {
                    x_spawn = x_pos;
                    y_spawn = y_pos+(y_size/2);
                    speed = -5;
                }
                entity_pool.push_back(std::make_unique<Bullet>(next_id, x_spawn, y_spawn, speed));
                updates.push_back(Update::Update_new::create_create_entity(
                        next_id,
                        Update::EntityType::Bullet,
                        Update::EntitySubtype::No_subtype
                ));
                next_id++;
                if (ammo_type != enums_value_update::Ammo_type::NORMAL) {
                    ammo[ammo_type]--;
                }
                break;

            case SPECIAL:
                // Not implemented: SPECIAL action
                break;

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
        return updates;
    };

    std::vector<Update::Update_new> tick(const Map& map,
        std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool) override {
        std::vector<Update::Update_new> updates;

        if (!is_active) {
            if (std::chrono::steady_clock::now() >= inactive_time + std::chrono::seconds(SECONDS_UNTIL_RESPAWN)) {
                revive(map.get_player_spawns());
                updates.push_back(Update::Update_new::create_position(
                        static_cast<uint16_t>(id),
                        x_pos,
                        y_pos));
            }
            return updates;
        }

        if (!is_damageable && std::chrono::steady_clock::now() >= (inactive_time + std::chrono::seconds(SECONDS_IMMUNE_AFTER_DAMAGE))) {
            is_damageable = true;
        }

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
                    case LIGHT_AMMO:
                        ammo[enums_value_update::LIGHT] += pickup->getValue();
                        break;
                    case HEAVY_AMMO:
                        ammo[enums_value_update::HEAVY] += pickup->getValue();
                        break;
                    case POWER_AMMO:
                        ammo[enums_value_update::POWER] += pickup->getValue();
                        break;
                }
                updates.push_back(Update::Update_new::create_delete_entity(other->get_id()));
                delete_pickup(entity_pool, pickup->get_id());
            }
        }

        return updates;
    }

    void revive(std::vector<Coordinate> spawns) {
        // TODO: send update...? Position for sure, but events?
        Coordinate spawn = spawns[rand() % spawns.size()];
        x_pos = spawn.x;
        y_pos = spawn.y;

        health = PLAYER_HEALTH;
        is_active = true;
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
