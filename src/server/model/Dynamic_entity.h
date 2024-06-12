#ifndef DYNAMIC_ENTITY_H
#define DYNAMIC_ENTITY_H

#include <stdexcept>
#include <vector>

#include "Game.h"
#include "physics/physics.h"

class Dynamic_entity : public RigidBox {
protected:
    int id;

    int vel_x;
    int vel_y;

    // esto probablemente sea agregado cuando implementemos física
    // (para la gravedad seguro y estamos evaluando hacerlo para movimiento en X)
    // int acc_x;
    // int acc_y;

    // esto lo usamos para diferenciar una bala de un jugador, si no hace daño se setea en 0
    // (para evitar tener 2 atributos, uno boolean y otro con el valor)
    bool is_damageable;
    int damage_on_contact;
    bool destroyed_on_contact;
public:
    Dynamic_entity(int id, int pos_x, int pos_y, int width, int height, int vel_x, int vel_y,
                   /*, int acc_x, int acc_y,*/ bool is_damageable, int damage_on_contact, bool destroyed_on_contact)
        : id(id),
          RigidBox(pos_x, pos_y, width, height),
          vel_x(vel_x),
          vel_y(vel_y), /*,
           acc_x(acc_x),
           acc_y(acc_y),*/
          is_damageable(is_damageable),
          damage_on_contact(damage_on_contact),
          destroyed_on_contact(destroyed_on_contact)
          {};

    virtual std::vector<Update> tick(
        std::vector<std::unique_ptr<Dynamic_entity>>* entity_pool) = 0;

    void setXSpeed(int vel_x_param) { vel_x = vel_x_param; }
    int getXSpeed() { return vel_x; }

    void setYSpeed(int vel_y_param) { vel_y = vel_y_param; }
    int getYSpeed() { return vel_y; }

    bool is_entity_damageable() const { return is_damageable; }
    int get_damage_dealt() const { return damage_on_contact; }
};

#endif  // DYNAMIC_ENTITY_H
