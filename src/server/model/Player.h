#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>
#include <map>

#include "../../common/Update.h"
#include "Bullet.h"
#include "Dynamic_entity.h"
#include "Pickup.h"
#include "constants/pickup_type.h"
#include "Ammo_data.h"
#include "../loader/config.h"

#define PLAYER_HEIGHT 2
#define PLAYER_WIDTH 1
#define PLAYER_INITIAL_X_VEL 0
#define PLAYER_INITIAL_Y_VEL 0
#define GRAVITY 0.05
#define SECONDS_UNTIL_RESPAWN 3
#define SECONDS_IMMUNE_AFTER_DAMAGE 2

#define MILLISECONDS_IN_A_MINUTE 60000

class Player : public Dynamic_entity {
   private:
    int points;
    std::map<enums_value_update::Ammo_type, int> ammo;
    enums_value_update::Ammo_type current_ammo_type;
    std::map<enums_value_update::Ammo_type, enums_value_update::Ammo_type> next_ammo_type;
    std::map<enums_value_update::Ammo_type, Ammo> ammo_config;

    bool is_shooting;
    std::chrono::steady_clock::time_point last_shot_time;
    Update::EntitySubtype type;
public:
    Player(int id, float x_spawn, float y_spawn, Update::EntitySubtype type)
        : Dynamic_entity(id, x_spawn, y_spawn, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_INITIAL_X_VEL, PLAYER_INITIAL_Y_VEL, GRAVITY, true, 0, false, Config::get_player_max_health(), true, true),
          points(0), current_ammo_type(enums_value_update::Ammo_type::NORMAL), is_shooting(false), last_shot_time(std::chrono::steady_clock::time_point()), type(type){
        ammo[enums_value_update::Ammo_type::LIGHT] = 0;
        ammo[enums_value_update::Ammo_type::HEAVY] = 0;
        ammo[enums_value_update::Ammo_type::POWER] = 0;

        next_ammo_type[enums_value_update::Ammo_type::NORMAL] = enums_value_update::Ammo_type::LIGHT;
        next_ammo_type[enums_value_update::Ammo_type::LIGHT] = enums_value_update::Ammo_type::HEAVY;
        next_ammo_type[enums_value_update::Ammo_type::HEAVY] = enums_value_update::Ammo_type::POWER;
        next_ammo_type[enums_value_update::Ammo_type::POWER] = enums_value_update::Ammo_type::NORMAL;

        ammo_config[enums_value_update::Ammo_type::NORMAL] = Ammo::create_normal();
        ammo_config[enums_value_update::Ammo_type::LIGHT] = Ammo::create_light();
        ammo_config[enums_value_update::Ammo_type::HEAVY] = Ammo::create_heavy();
        ammo_config[enums_value_update::Ammo_type::POWER] = Ammo::create_power();
    };

    std::vector<Update::Update_new> tick(const Map& map,
        std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool, int& next_id) override {
        std::vector<Update::Update_new> updates;

        if (!is_active) {
            if (std::chrono::steady_clock::now() >=
                inactive_time + std::chrono::seconds(SECONDS_UNTIL_RESPAWN)) {
                revive(map.get_player_spawns());
                updates.push_back(Update::Update_new::create_position(
                    static_cast<uint16_t>(id), x_pos, y_pos));
            }
            return updates;
        }

        if (!is_damageable && std::chrono::steady_clock::now() >= (inactive_time + std::chrono::seconds(SECONDS_IMMUNE_AFTER_DAMAGE))) {
            is_damageable = true;
        }

        if (is_shooting && std::chrono::steady_clock::now() >= (last_shot_time + std::chrono::milliseconds(MILLISECONDS_IN_A_MINUTE / ammo_config[current_ammo_type].get_rate_of_fire()))) {
            std::vector<Update::Update_new> shoot_update = shoot(entity_pool, next_id);
            last_shot_time = std::chrono::steady_clock::now();
            updates.insert(updates.end(), shoot_update.begin(), shoot_update.end());
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
                static_cast<uint16_t>(id), x_pos, y_pos);
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
                        if (ammo[enums_value_update::LIGHT] > ammo_config[enums_value_update::LIGHT].get_max_ammo()) {
                            ammo[enums_value_update::LIGHT] = ammo_config[enums_value_update::LIGHT].get_max_ammo();
                        }
                        break;
                    case HEAVY_AMMO:
                        ammo[enums_value_update::HEAVY] += pickup->getValue();
                        if (ammo[enums_value_update::HEAVY] > ammo_config[enums_value_update::HEAVY].get_max_ammo()) {
                            ammo[enums_value_update::HEAVY] = ammo_config[enums_value_update::HEAVY].get_max_ammo();
                        }
                        break;
                    case POWER_AMMO:
                        ammo[enums_value_update::POWER] += pickup->getValue();
                        if (ammo[enums_value_update::HEAVY] > ammo_config[enums_value_update::HEAVY].get_max_ammo()) {
                            ammo[enums_value_update::HEAVY] = ammo_config[enums_value_update::HEAVY].get_max_ammo();
                        }
                        break;
                }
                updates.push_back(Update::Update_new::create_delete_entity(other->get_id()));
                delete_pickup(entity_pool, pickup->get_id());
            }
        }
        return updates;
    }

    std::vector<Update::Update_new> process_action(uint8_t action, std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool, int& next_id) {
        std::vector<Update::Update_new> total_updates;
        std::vector<Update::Update_new> action_updates;
        switch (action) {
            case JUMP:
                setYSpeed(Config::get_player_jump() * (-1));
                break;
            case RUN_LEFT:
                setXSpeed(Config::get_player_speed() * (-1));
                break;

            case RUN_RIGHT:
                setXSpeed(Config::get_player_speed());
                break;

            case SHOOT:
                action_updates = shoot(entity_pool, next_id);
                total_updates.insert(total_updates.end(), action_updates.begin(),action_updates.end());
                break;

            case STOP_SHOOT:
                is_shooting = false;
                break;

            case SWITCH_GUN:
                current_ammo_type = get_next_ammo_type(current_ammo_type);
                total_updates.push_back(Update::Update_new::create_value(
                        id,
                        Update::UpdateType::ChangeAmmoType,
                        current_ammo_type
                ));
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
        return total_updates;
    };

    std::vector<Update::Update_new> shoot(std::vector<std::unique_ptr<Dynamic_entity>> &entity_pool, int &next_id) {
        std::vector<Update::Update_new> updates;

        if (current_ammo_type != enums_value_update::Ammo_type::NORMAL && ammo[current_ammo_type] <= 0) {
            return updates;
        }
        float x_spawn = x_pos;
        float y_spawn = y_pos+(y_size/2);
        float speed = ammo_config[current_ammo_type].get_speed();
        int damage = ammo_config[current_ammo_type].get_damage();

        if (looking_right) {
            x_spawn += x_size;
        } else {
            speed *= -1;
        }

        entity_pool.push_back(std::make_unique<Bullet>(next_id, x_spawn, y_spawn, speed, damage));
        updates.push_back(Update::Update_new::create_create_entity(
                next_id,
                Update::EntityType::Bullet,
                Update::EntitySubtype::No_subtype
        ));
        next_id++;
        if (current_ammo_type != enums_value_update::Ammo_type::NORMAL) {
            ammo[current_ammo_type]--;
        }

        return updates;
    }

    enums_value_update::Ammo_type get_next_ammo_type(enums_value_update::Ammo_type ammo_type) {
        enums_value_update::Ammo_type result = next_ammo_type[ammo_type];
        if (ammo[result] == enums_value_update::Ammo_type::NORMAL || ammo[result] > 0) {
            return result;
        }
        return get_next_ammo_type(result);
    }

    void revive(std::vector<Coordinate> spawns) {
        // TODO: send update...? Position for sure, but events?
        Coordinate spawn = spawns[rand() % spawns.size()];
        x_pos = spawn.x;
        y_pos = spawn.y;

        health = Config::get_player_max_health();
        is_active = true;
    }

    void delete_pickup(std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool, int pickup_id) {
        auto it = std::find_if(entity_pool.begin(), entity_pool.end(),
                               [pickup_id](const std::unique_ptr<Dynamic_entity>& entity) {
                                   return entity->get_id() == pickup_id;
                               });
        if (it != entity_pool.end()) {
            entity_pool.erase(it);
        }
    }

    Update::EntitySubtype get_player_subtype(){return type;}
};

#endif  // PLAYER_H
