#ifndef PLAYER_H
#define PLAYER_H

#include <algorithm>
#include <cmath>
#include <map>

#include "../../common/Update.h"
#include "../loader/config.h"
#include "Ammo_data.h"
#include "Bullet.h"
#include "Dynamic_entity.h"
#include "Pickup.h"
#include "constants/pickup_type.h"

#define PLAYER_HEIGHT 1.33
#define PLAYER_WIDTH 0.8
#define PLAYER_INITIAL_X_VEL 0
#define PLAYER_INITIAL_Y_VEL 0
#define GRAVITY 0.05
#define SECONDS_UNTIL_RESPAWN 3
#define SECONDS_IMMUNE_AFTER_DAMAGE 2
#define SPECIAL_ATTACK_DAMAGE 30
#define SPECIAL_ATTACK_SPEED 2

#define MILLISECONDS_IN_A_MINUTE 60000

class Player : public Dynamic_entity {
   private:
    int points;
    Update::EntitySubtype type;
    std::map<enums_value_update::Ammo_type, int> ammo;
    enums_value_update::Ammo_type current_ammo_type;
    std::map<enums_value_update::Ammo_type, enums_value_update::Ammo_type>
        next_ammo_type;
    std::map<enums_value_update::Ammo_type, Ammo> ammo_config;

    enums_value_update::Player_State_Enum current_state;
    bool is_shooting;
    bool is_doing_special;
    bool is_running;
    bool is_jumping;
    bool is_falling;

    bool is_x_move_blocked;
    bool is_y_move_blocked;
    std::chrono::steady_clock::time_point last_shot_time;

   public:
    Player(int id, float x_spawn, float y_spawn, Update::EntitySubtype type)
        : Dynamic_entity(id, x_spawn, y_spawn, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_INITIAL_X_VEL, PLAYER_INITIAL_Y_VEL, 0, GRAVITY, true, 0, false, Config::get_player_max_health(), true),
          points(0), type(type), current_ammo_type(enums_value_update::Ammo_type::NORMAL), current_state(enums_value_update::Player_State_Enum::Idle), is_shooting(false), is_doing_special(false), is_running(false), is_jumping(false), is_falling(false), is_x_move_blocked(false), is_y_move_blocked(false), last_shot_time(std::chrono::steady_clock::time_point()) {
        ammo[enums_value_update::Ammo_type::NORMAL] = 255;
        ammo[enums_value_update::Ammo_type::LIGHT] = 0;
        ammo[enums_value_update::Ammo_type::HEAVY] = 0;
        ammo[enums_value_update::Ammo_type::POWER] = 0;

        next_ammo_type[enums_value_update::Ammo_type::NORMAL] = enums_value_update::Ammo_type::LIGHT;
        next_ammo_type[enums_value_update::Ammo_type::LIGHT] = enums_value_update::Ammo_type::HEAVY;
        next_ammo_type[enums_value_update::Ammo_type::HEAVY] = enums_value_update::Ammo_type::POWER;
        next_ammo_type[enums_value_update::Ammo_type::POWER] = enums_value_update::Ammo_type::NORMAL;

        ammo_config[enums_value_update::Ammo_type::NORMAL] =
            Ammo::create_normal();
        ammo_config[enums_value_update::Ammo_type::LIGHT] =
            Ammo::create_light();
        ammo_config[enums_value_update::Ammo_type::HEAVY] =
            Ammo::create_heavy();
        ammo_config[enums_value_update::Ammo_type::POWER] =
            Ammo::create_power();
    };

    std::vector<Update::Update_new> tick(
        const Map& map,
        std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool,
        int& next_id) override {
        std::vector<Update::Update_new> updates;

        if (!is_active) {
            if (std::chrono::steady_clock::now() >=
                inactive_time + std::chrono::seconds(SECONDS_UNTIL_RESPAWN)) {
                revive(map.get_player_spawns());

                auto [x_client, y_client] = get_position_for_client();

                updates.push_back(Update::Update_new::create_position(
                    static_cast<uint16_t>(id), x_client, y_client));
                current_state = enums_value_update::Player_State_Enum::Idle;
                updates.push_back(Update::Update_new::create_value(
                        id, Update::UpdateType::State, current_state));
            }
            return updates;
        }

        if (!is_damageable && !is_doing_special &&
            std::chrono::steady_clock::now() >=
                (inactive_time +
                 std::chrono::seconds(SECONDS_IMMUNE_AFTER_DAMAGE))) {
            is_damageable = true;
        }

        if (is_shooting &&
            std::chrono::steady_clock::now() >=
                (last_shot_time +
                 std::chrono::milliseconds(
                     MILLISECONDS_IN_A_MINUTE /
                     ammo_config[current_ammo_type].get_rate_of_fire()))) {
            std::vector<Update::Update_new> shoot_update =
                shoot(entity_pool, next_id);
            last_shot_time = std::chrono::steady_clock::now();
            updates.insert(updates.end(), shoot_update.begin(),
                           shoot_update.end());
        }

        float old_x = x_pos;
        float old_y = y_pos;

        vel_y += acc_y;
        vel_x += acc_x;

        // validar movimiento en X
        if (vel_x != 0) {
            x_pos += vel_x;

            if (is_doing_special && ((direction == enums_value_update::Direction::Right && vel_x < 0)
            || (direction == enums_value_update::Direction::Left && vel_x > 0))) {
                is_doing_special = false;
                damage_on_contact = 0;
                is_damageable = true;
                is_x_move_blocked = false;
                is_y_move_blocked = false;
                vel_x = 0;
                acc_x = 0;
            }

            if (collides_with_map(map)) {
                x_pos -= vel_x;
            }
        }

        if (vel_y != 0) {
            y_pos += vel_y;

            if (collides_with_map(map)) {
                y_pos -= vel_y;
                if (vel_y > 0) {
                    is_y_move_blocked = false;
                }
                vel_y = 0;
            }
            if (vel_y > 0) {
                if (((type == Update::EntitySubtype::Jazz || type == Update::EntitySubtype::Lori) && is_doing_special) || is_jumping) {
                    reset_special_state();
                }
                is_falling = true;
                is_jumping = false;
            } else if (vel_y < 0) {
                is_falling = false;
                is_jumping = true;
            } else {
                if (((type == Update::EntitySubtype::Jazz || type == Update::EntitySubtype::Lori) && is_doing_special) || is_jumping) {
                    reset_special_state();
                }
                is_falling = false;
                is_jumping = false;
            }
        }

        if (x_pos != old_x || y_pos != old_y) {
            auto [x_client, y_client] = get_position_for_client();

            Update::Update_new update = Update::Update_new::create_position(
                static_cast<uint16_t>(id), x_client, y_client);
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
                        if (ammo[enums_value_update::LIGHT] >
                            ammo_config[enums_value_update::LIGHT]
                                .get_max_ammo()) {
                            ammo[enums_value_update::LIGHT] =
                                ammo_config[enums_value_update::LIGHT]
                                    .get_max_ammo();
                        }
                        break;
                    case HEAVY_AMMO:
                        ammo[enums_value_update::HEAVY] += pickup->getValue();
                        if (ammo[enums_value_update::HEAVY] >
                            ammo_config[enums_value_update::HEAVY]
                                .get_max_ammo()) {
                            ammo[enums_value_update::HEAVY] =
                                ammo_config[enums_value_update::HEAVY]
                                    .get_max_ammo();
                        }
                        break;
                    case POWER_AMMO:
                        ammo[enums_value_update::POWER] += pickup->getValue();
                        if (ammo[enums_value_update::HEAVY] >
                            ammo_config[enums_value_update::HEAVY]
                                .get_max_ammo()) {
                            ammo[enums_value_update::HEAVY] =
                                ammo_config[enums_value_update::HEAVY]
                                    .get_max_ammo();
                        }
                        break;
                }
                updates.push_back(
                    Update::Update_new::create_delete_entity(other->get_id()));
                delete_pickup(entity_pool, pickup->get_id());
            }
        }

        enums_value_update::Player_State_Enum new_player_state =
            get_player_state();
        if (new_player_state != current_state) {
            current_state = new_player_state;
            updates.push_back(Update::Update_new::create_value(
                id, Update::UpdateType::State, new_player_state));
        }
        return updates;
    }

    std::vector<Update::Update_new> process_action(
        uint8_t action,
        std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool,
        int& next_id) {
        std::vector<Update::Update_new> total_updates;
        std::vector<Update::Update_new> action_updates;
        switch (action) {
            case JUMP:
                if (!is_y_move_blocked) {
                    setYSpeed(Config::get_player_jump() * (-1));
                    is_jumping = true;
                    is_y_move_blocked = true;
                }
                break;
            case RUN_LEFT:
                if (!is_x_move_blocked) {
                    direction = enums_value_update::Direction::Left;
                    setXSpeed(Config::get_player_speed() * (-1));
                    total_updates.push_back(Update::Update_new::create_value(
                            id,
                            Update::UpdateType::Direction,
                            direction
                    ));
                    is_running = true;
                }
                break;

            case RUN_RIGHT:
                if (!is_x_move_blocked) {
                    direction = enums_value_update::Direction::Right;
                    setXSpeed(Config::get_player_speed());
                    total_updates.push_back(Update::Update_new::create_value(
                            id,
                            Update::UpdateType::Direction,
                            direction
                    ));
                    is_running = true;
                }
                break;

            case SHOOT:
                is_shooting = true;
                action_updates = shoot(entity_pool, next_id);
                last_shot_time = std::chrono::steady_clock::now();
                total_updates.insert(total_updates.end(),
                                     action_updates.begin(),
                                     action_updates.end());
                break;

            case STOP_SHOOT:
                is_shooting = false;
                break;

            case SWITCH_GUN:
                current_ammo_type = get_next_ammo_type(current_ammo_type);
                total_updates.push_back(Update::Update_new::create_value(
                    id, Update::UpdateType::ChangeAmmoType, current_ammo_type));
                total_updates.push_back(Update::Update_new::create_value(
                        id,
                        Update::BulletsRemaining,
                        static_cast<uint8_t>(ammo[current_ammo_type])
                ));
                break;

            case SPECIAL:
                action_updates = special_action();
                total_updates.insert(total_updates.end(), action_updates.begin(),action_updates.end());
                break;

            case STOP_RUN_RIGHT:
                if (vel_x > 0 || !(type == Update::EntitySubtype::Spaz && is_doing_special)) {
                    vel_x = 0;
                    is_running = false;
                }
                break;

            case STOP_RUN_LEFT:
                if (vel_x < 0 || !(type == Update::EntitySubtype::Spaz && is_doing_special)) {
                    vel_x = 0;
                    is_running = false;
                }
                break;

            default:
                // Handle unexpected actions if necessary
                break;
        }
        return total_updates;
    };

    std::vector<Update::Update_new> shoot(
        std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool,
        int& next_id) {
        std::vector<Update::Update_new> updates;

        if (current_ammo_type != enums_value_update::Ammo_type::NORMAL &&
            ammo[current_ammo_type] <= 0) {
            return updates;
        }
        float x_spawn = x_pos - 0.01f;
        float y_spawn = y_pos + (y_size / 2);
        float speed = ammo_config[current_ammo_type].get_speed();
        int damage = ammo_config[current_ammo_type].get_damage();

        if (direction == enums_value_update::Direction::Right) {
            x_spawn += x_size + 0.01f;
        } else {
            speed *= -1;
        }

        entity_pool.push_back(
            std::make_unique<Bullet>(next_id, x_spawn, y_spawn, speed, damage, id));
        updates.push_back(Update::Update_new::create_create_entity(
            next_id, Update::EntityType::Bullet,
            Update::EntitySubtype::No_subtype));

        if (speed > 0) {
            direction = enums_value_update::Direction::Right;
            updates.push_back(Update::Update_new::create_value(
                next_id, Update::UpdateType::Direction, direction));
        } else {
            direction = enums_value_update::Direction::Left;
            updates.push_back(Update::Update_new::create_value(
                next_id, Update::UpdateType::Direction, direction));
        }

        next_id++;
        if (current_ammo_type != enums_value_update::Ammo_type::NORMAL) {
            ammo[current_ammo_type]--;
            updates.push_back(Update::Update_new::create_value(
                    id,
                    Update::BulletsRemaining,
                    static_cast<uint8_t>(ammo[current_ammo_type])
                    ));
        }

        return updates;
    }

    enums_value_update::Ammo_type get_next_ammo_type(
        enums_value_update::Ammo_type ammo_type) {
        enums_value_update::Ammo_type result = next_ammo_type[ammo_type];
        if (result == enums_value_update::Ammo_type::NORMAL ||
            ammo[result] > 0) {
            return result;
        }
        return get_next_ammo_type(result);
    }

    void revive(std::vector<Coordinate> spawns) {
        Coordinate spawn = spawns[rand() % spawns.size()];
        x_pos = spawn.x;
        y_pos = spawn.y;

        health = Config::get_player_max_health();
        is_active = true;
        is_shooting = false;
        is_doing_special = false;
        is_running = false;
        is_jumping = false;
        is_falling = false;
        is_damageable = true;
        is_x_move_blocked = false;
        is_y_move_blocked = false;
    }

    std::vector<Update::Update_new> special_action() {
        std::vector<Update::Update_new> updates;

        if (!is_active || !is_damageable) {
            return updates;
        }

        switch (type) {
            case Update::EntitySubtype::Jazz:
                if (!is_y_move_blocked) {
                    is_doing_special = true;
                    damage_on_contact = SPECIAL_ATTACK_DAMAGE;
                    is_jumping = true;
                    is_damageable = false;
                    is_x_move_blocked = true;
                    is_y_move_blocked = true;
                    setYSpeed(SPECIAL_ATTACK_SPEED * (-1));
                }
                break;
            case Update::EntitySubtype::Spaz:
                if (!is_x_move_blocked) {
                    is_doing_special = true;
                    damage_on_contact = SPECIAL_ATTACK_DAMAGE;
                    is_damageable = false;
                    is_x_move_blocked = true;
                    is_y_move_blocked = true;
                    setXSpeed(Config::get_player_speed() * (direction == enums_value_update::Direction::Left ? -3.0f : 3.0f));
                    acc_x = GRAVITY * (direction == enums_value_update::Direction::Right ? -1.0f : 1.0f);
                }
                break;
            case Update::EntitySubtype::Lori:
                if (!is_y_move_blocked) {
                    is_doing_special = true;
                    damage_on_contact = SPECIAL_ATTACK_DAMAGE;
                    is_jumping = true;
                    is_damageable = false;
                    is_y_move_blocked = true;
                    setYSpeed(SPECIAL_ATTACK_SPEED * (-1));
                }
                break;
            default:
                break;
        }




        return updates;
    }

    enums_value_update::Player_State_Enum get_player_state() {
        if (!is_damageable) {
            return enums_value_update::Player_State_Enum::TakingDamage;
        } else if (is_shooting) {
            return enums_value_update::Player_State_Enum::Shooting;
        } else if (is_doing_special) {
            return enums_value_update::Player_State_Enum::SpecialAttack;
        } else if (is_jumping) {
            return enums_value_update::Player_State_Enum::Jumping;
        } else if (is_falling) {
            return enums_value_update::Player_State_Enum::Falling;
        } else if (is_running) {
            return enums_value_update::Player_State_Enum::Running;
        } else {
            return enums_value_update::Player_State_Enum::Idle;
        }
    }

    void reset_special_state() {
        is_doing_special = false;
        damage_on_contact = 0;
        is_damageable = true;
        is_x_move_blocked = false;
    }

    void delete_pickup(
        std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool,
        int pickup_id) {
        auto it = std::find_if(
            entity_pool.begin(), entity_pool.end(),
            [pickup_id](const std::unique_ptr<Dynamic_entity>& entity) {
                return entity->get_id() == pickup_id;
            });
        if (it != entity_pool.end()) {
            entity_pool.erase(it);
        }
    }

    virtual std::vector<Update::Update_new> handle_death(std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool, int& next_id) {
        std::vector<Update::Update_new> updates;
        updates.push_back(Update::Update_new::create_value(
                static_cast<uint16_t>(id),
                Update::UpdateType::Health,
                static_cast<uint8_t>(health)));
        updates.push_back(Update::Update_new::create_value(
                static_cast<uint16_t>(id),
                Update::UpdateType::State,
                enums_value_update::Player_State_Enum::Dead));
        is_active = false;
        inactive_time = std::chrono::steady_clock::now();
        return updates;
    }

    Update::EntitySubtype get_player_subtype() { return type; }

    void increase_points(int more_points) { points += more_points; }
    virtual int get_points() { return points; };
};

#endif  // PLAYER_H
