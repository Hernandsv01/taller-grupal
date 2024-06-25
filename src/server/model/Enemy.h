#ifndef ENEMY_H
#define ENEMY_H

#include "Dynamic_entity.h"
#include "Player.h"

#define ENEMY_HEIGHT 0.5
#define ENEMY_WIDTH 0.2
#define ENEMY_MAX_MOVEMENT_RANGE 50
#define SECONDS_UNTIL_RESPAWN 3

class Enemy : public Dynamic_entity {
   private:
    int movement_range;
    Update::EntitySubtype subtype;

   public:
    Enemy(int id, float x_spawn, float y_spawn, Update::EntitySubtype subtype)
        : Dynamic_entity(id, x_spawn, y_spawn, ENEMY_WIDTH, ENEMY_HEIGHT,
                         Config::get_crawler_speed(), 0, GRAVITY, true,
                         Config::get_crawler_damage(), false,
                         Config::get_crawler_life(), true),
          movement_range(ENEMY_MAX_MOVEMENT_RANGE),
          subtype(subtype) {}
    std::vector<Update::Update_new> tick(
        const Map& map,
        std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool,
        int& next_id) override {
        std::vector<Update::Update_new> updates;

        if (!is_active) {
            if (std::chrono::steady_clock::now() >=
                inactive_time + std::chrono::seconds(SECONDS_UNTIL_RESPAWN)) {
                revive(map.get_enemy_spawns());
                updates.push_back(Update::Update_new::create_position(
                    static_cast<uint16_t>(id), x_pos, y_pos));
            }
            return updates;
        }

        is_damageable = true;

        float old_x = x_pos;
        float old_y = y_pos;

        vel_y += acc_y;

        // validar movimiento en X
        if (vel_x != 0) {
            x_pos += vel_x;

            if (collides_with_map(map)) {
                x_pos -= vel_x;
                vel_x *= (-1);
            }

            if (direction == enums_value_update::Direction::Right &&
                vel_x < 0) {
                direction = enums_value_update::Direction::Left;
                updates.push_back(Update::Update_new::create_value(
                    id, Update::UpdateType::Direction, direction));
            } else if (direction == enums_value_update::Direction::Left &&
                       vel_x > 0) {
                direction = enums_value_update::Direction::Right;
                updates.push_back(Update::Update_new::create_value(
                    id, Update::UpdateType::Direction, direction));
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

            if (dynamic_cast<Player*>(other.get()) &&
                other->is_entity_damageable()) {
                bool is_dead = other->deal_damage(get_damage_dealt());

                if (is_dead) {
                    updates.push_back(Update::Update_new::create_value(
                        static_cast<uint16_t>(other->get_id()),
                        Update::UpdateType::State,
                        enums_value_update::Player_State_Enum::Dead));
                } else {
                    updates.push_back(Update::Update_new::create_value(
                        static_cast<uint16_t>(other->get_id()),
                        Update::UpdateType::Health,
                        static_cast<uint8_t>(other->get_health())));
                }

                return updates;
            }
        }

        return updates;
    }

    void revive(std::vector<Coordinate> spawns) {
        Coordinate spawn = spawns[rand() % spawns.size()];
        x_pos = spawn.x;
        y_pos = spawn.y;

        health = Config::get_crawler_life();
        is_active = true;
        is_damageable = true;
    }

    Update::EntitySubtype get_subtype() { return subtype; }
};

#endif  // ENEMY_H
