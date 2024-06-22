#ifndef DYNAMIC_ENTITY_H
#define DYNAMIC_ENTITY_H

#include <cmath>
#include <stdexcept>
#include <vector>

#include "Game.h"
#include "map/map.h"
#include "physics/physics.h"

class Dynamic_entity : public RigidBox {
   protected:
    int id;

    float vel_x;
    float vel_y;

    // esto probablemente sea agregado cuando implementemos física
    // (para la gravedad seguro y estamos evaluando hacerlo para movimiento en
    // X) int acc_x;
    float acc_y;

    // esto lo usamos para diferenciar una bala de un jugador, si no hace daño
    // se setea en 0 (para evitar tener 2 atributos, uno boolean y otro con el
    // valor)
    bool is_damageable;
    std::chrono::steady_clock::time_point last_damaged;
    int damage_on_contact;

    bool is_item;

    int health;
    bool is_active;
    std::chrono::steady_clock::time_point inactive_time;

    bool looking_right;

   public:
    Dynamic_entity(int id, float pos_x, float pos_y, float width, float height,
                   float vel_x, float vel_y, float acc_y, bool is_damageable,
                   int damage_on_contact, bool is_item, int health,
                   bool is_active, bool looking_right)
        : id(id),
          RigidBox(pos_x, pos_y, width, height),
          vel_x(vel_x),
          vel_y(vel_y),
          acc_y(acc_y),
          is_damageable(is_damageable),
          last_damaged(std::chrono::steady_clock::time_point()),
          damage_on_contact(damage_on_contact),
          is_item(is_item),
          health(health),
          is_active(is_active),
          inactive_time(std::chrono::steady_clock::time_point()),
          looking_right(looking_right){};

    ~Dynamic_entity(){};

    virtual std::vector<Update::Update_new> tick(const Map& map,
        std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool, int& next_id) = 0;

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
                if (x < 0 || x >= map.get_map_size().x || y < 0 || y >= map.get_map_size().y) {
                    return true;
                }
                Collision collision = map.get_block_collision(
                    {static_cast<uint8_t>(x), static_cast<uint8_t>(y)});
                if (collision != Collision::Air) {
                    return true;
                }
            }
        }
        return false;
    }
};

#endif  // DYNAMIC_ENTITY_H
