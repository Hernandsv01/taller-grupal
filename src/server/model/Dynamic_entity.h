#ifndef DYNAMIC_ENTITY_H
#define DYNAMIC_ENTITY_H

#include <stdexcept>
#include <vector>

#include "Game.h"
#include "physics/physics.h"
#include "map/map.h"

class Dynamic_entity : public RigidBox {
protected:
    int id;

    float vel_x;
    float vel_y;

    // esto probablemente sea agregado cuando implementemos física
    // (para la gravedad seguro y estamos evaluando hacerlo para movimiento en X)
    // int acc_x;
    float acc_y;

    // esto lo usamos para diferenciar una bala de un jugador, si no hace daño se setea en 0
    // (para evitar tener 2 atributos, uno boolean y otro con el valor)
    bool is_damageable;
    int damage_on_contact;

    bool is_item;

    int health;
public:
    Dynamic_entity(int id, float pos_x, float pos_y, float width, float height, float vel_x, float vel_y,
                   /*, int acc_x,*/ float acc_y, bool is_damageable, int damage_on_contact, bool is_item, int health)
        : id(id),
          RigidBox(pos_x, pos_y, width, height),
          vel_x(vel_x),
          vel_y(vel_y),
          //acc_x(acc_x),
          acc_y(acc_y),
          is_damageable(is_damageable),
          damage_on_contact(damage_on_contact),
          is_item(is_item),
          health(health)
          {};

    virtual std::vector<Update> tick(Map map,
        std::vector<std::unique_ptr<Dynamic_entity>>* entity_pool) = 0;

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
        return health <= 0;
    };
};

#endif  // DYNAMIC_ENTITY_H
