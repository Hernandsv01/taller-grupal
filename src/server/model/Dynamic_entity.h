#ifndef DYNAMIC_ENTITY_H
#define DYNAMIC_ENTITY_H

#include <vector>
#include "Entity.h"
#include "Game.h"

class Dynamic_entity : public Entity {
protected:
    int id;

    int vel_x;
    int vel_y;

    // int acc_x;
    // int acc_y;
public:
    Dynamic_entity(int id, int pos_x, int pos_y, int width, int height, bool is_damageable, int damage_on_contact, int vel_x, int vel_y/*, int acc_x, int acc_y*/) :
            id(id),
            Entity(pos_x, pos_y, width, height, is_damageable, damage_on_contact),
            vel_x(vel_x),
            vel_y(vel_y)/*,
            acc_x(acc_x),
            acc_y(acc_y)*/ {};

    virtual std::vector<Update> tick(std::vector<Dynamic_entity>* entity_pool);

    void setXSpeed(int vel_x_param) {
        vel_x = vel_x_param;
    }

    void setYSpeed(int vel_y_param) {
        vel_y = vel_y_param;
    }
};

#endif //DYNAMIC_ENTITY_H
