#ifndef DYNAMIC_ENTITY_H
#define DYNAMIC_ENTITY_H

#include <stdexcept>
#include <vector>

#include "Entity.h"
#include "Game.h"

class Dynamic_entity : public Entity {
   protected:
    int id;

    int vel_x;
    int vel_y;

    // esto probablemente sea agregado cuando implementemos física
    // (para la gravedad seguro y estamos evaluando hacerlo para movimiento en X)
    // int acc_x;
    // int acc_y;
   public:
    Dynamic_entity(int id, int pos_x, int pos_y, int width, int height,
                   bool is_damageable, int damage_on_contact, int vel_x,
                   int vel_y /*, int acc_x, int acc_y*/)
        : id(id),
          Entity(pos_x, pos_y, width, height, is_damageable, damage_on_contact),
          vel_x(vel_x),
          vel_y(vel_y) /*,
           acc_x(acc_x),
           acc_y(acc_y)*/
          {};

    virtual std::vector<Update> tick(
        std::vector<std::unique_ptr<Dynamic_entity>>* entity_pool) = 0;

    void setXSpeed(int vel_x_param) { vel_x = vel_x_param; }
    int getXSpeed() { return vel_x; }

    void setYSpeed(int vel_y_param) { vel_y = vel_y_param; }
    int getYSpeed() { return vel_y; }
};

#endif  // DYNAMIC_ENTITY_H
