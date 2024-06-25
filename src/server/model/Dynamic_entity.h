#ifndef DYNAMIC_ENTITY_H
#define DYNAMIC_ENTITY_H

#include <cmath>
#include <stdexcept>
#include <vector>

#include "../loader/config.h"
#include "Game.h"
#include "map/map.h"
#include "physics/physics.h"

class Dynamic_entity : public RigidBox {
   protected:
    int id;

    float vel_x;
    float vel_y;

    float acc_x;
    float acc_y;

    bool is_damageable;
    std::chrono::steady_clock::time_point last_damaged;
    int damage_on_contact;

    bool is_item;

    int health;
    bool is_active;
    bool pending_deletion;
    std::chrono::steady_clock::time_point inactive_time;
    std::chrono::steady_clock::time_point immune_time;

    enums_value_update::Direction direction;

   public:
    Dynamic_entity(int id, float pos_x, float pos_y, float width, float height,
                   float vel_x, float vel_y, float acc_x, float acc_y,
                   bool is_damageable, int damage_on_contact, bool is_item,
                   int health, bool is_active)
        : RigidBox(pos_x, pos_y - height - 0.01, width, height),
          id(id),
          vel_x(vel_x),
          vel_y(vel_y),
          acc_x(acc_x),
          acc_y(acc_y),
          is_damageable(is_damageable),
          last_damaged(std::chrono::steady_clock::time_point()),
          damage_on_contact(damage_on_contact),
          is_item(is_item),
          health(health),
          is_active(is_active),
          pending_deletion(false),
          inactive_time(std::chrono::steady_clock::time_point()),
          immune_time(std::chrono::steady_clock::time_point()),
          direction(enums_value_update::Direction::Right){};

    ~Dynamic_entity() = default;

    virtual std::vector<Update::Update_new> tick(
        const Map& map,
        std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool,
        int& next_id) = 0;

    void setXSpeed(float vel_x_param) { vel_x = vel_x_param; }
    float getXSpeed() const { return vel_x; }

    void setYSpeed(float vel_y_param) { vel_y = vel_y_param; }
    float getYSpeed() const { return vel_y; }

    void setYAcceleration(float acc_y_param) { acc_y = acc_y_param; }
    float getYAcceleration() const { return acc_y; }

    bool is_entity_damageable() const { return is_damageable; }
    bool is_dead() const { return health <= 0; }
    int get_damage_dealt() const { return damage_on_contact; }
    int get_id() const { return id; };
    bool get_is_item() const { return is_item; };
    bool deal_damage(int damage) {
        health -= damage;
        if (health <= 0) {
            is_active = false;
            inactive_time = std::chrono::steady_clock::now();
            return true;
        } else {
            immune_time = std::chrono::steady_clock::now();
            // cooldown of being hit
            is_damageable = false;
        }
        return false;
    };
    bool collides_with_map(const Map& map) {
        int x_min = static_cast<int>(std::floor(this->x_min()));
        int x_max = static_cast<int>(std::floor(this->x_max()));
        int y_min = static_cast<int>(std::floor(this->y_min()));
        int y_max = static_cast<int>(std::floor(this->y_max()));

        for (int x = x_min; x <= x_max; ++x) {
            for (int y = y_min; y <= y_max; ++y) {
                if (x < 0 || x >= map.get_map_size().x || y < 0 ||
                    y >= map.get_map_size().y) {
                    return true;
                }
                Collision collision = map.get_block_collision(
                    {static_cast<uint8_t>(x), static_cast<uint8_t>(y)});
                if (collision == Collision::Cube) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isPendingDeletion() { return pending_deletion; }

    void set_pending_deletion(){ pending_deletion = true; }

    int get_health() { return health; }

    std::pair<float, float> get_position_for_client() {
        // Obtengo la base en los pies.
        float x_for_client = x_pos + (x_size / 2);
        float y_for_client = y_pos + y_size;

        return std::make_pair(x_for_client, y_for_client);
    }


};

#endif  // DYNAMIC_ENTITY_H
