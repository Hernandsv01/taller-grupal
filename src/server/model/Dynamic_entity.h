#ifndef DYNAMIC_ENTITY_H
#define DYNAMIC_ENTITY_H

#include "Entity.h"

class Dynamic_entity : public Entity {
private:
    int vel_x;
    int vel_y;

    int acc_x;
    int acc_y;
public:
    Dynamic_entity(int pos_x, int pos_y, int width, int height, int vel_x, int vel_y, int acc_x, int acc_y) : Entity(pos_x, pos_y, width, height), vel_x(vel_x), vel_y(vel_y), acc_x(acc_x), acc_y(acc_y) {};
    Dynamic_entity(int pos_x, int pos_y, int width, int height) : Entity(pos_x, pos_y, width, height), vel_x(0), vel_y(0), acc_x(0), acc_y(0) {};

    std::vector<Update> tick();
    bool isMovementValid(/*RECIBIR TIPO DE MOVIMIENTO*/);
    bool checkCollisions();
};

#endif //DYNAMIC_ENTITY_H
