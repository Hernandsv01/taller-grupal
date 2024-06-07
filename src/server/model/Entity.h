#ifndef ENTITY_H
#define ENTITY_H

class Entity {
protected:
    int pos_x;
    int pos_y;

    int width;
    int height;

    // esto lo usamos para diferenciar una bala de un jugador, si no hace daño se setea en 0
    // (para evitar tener 2 atributos, uno boolean y otro con el valor)
    bool is_damageable;
    int damage_on_contact;
public:
    Entity(int pos_x, int pos_y, int width, int height, bool is_damageable, int damage_on_contact) : pos_x(pos_x), pos_y(pos_y), width(width), height(height), is_damageable(is_damageable), damage_on_contact(damage_on_contact) {};
};

#endif //ENTITY_H
