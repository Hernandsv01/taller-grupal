#ifndef AMMO_DATA_H
#define AMMO_DATA_H

#include "Update.h"

class Ammo {
private:
    enums_value_update::Ammo_type type;
    int rate_of_fire; // bullets per minute
    float speed;
    int damage;
    Ammo(enums_value_update::Ammo_type type, int rate_of_fire, float speed, int damage) : type(type), rate_of_fire(rate_of_fire), speed(speed), damage(damage) {}

public:
    Ammo() {}
    // TODO: Usar valores de la config
    static Ammo create_normal() { return {enums_value_update::NORMAL, 0, 0, 0}; }
    static Ammo create_light() { return {enums_value_update::LIGHT, 0, 0, 0}; }
    static Ammo create_heavy() { return {enums_value_update::HEAVY, 0, 0, 0}; }
    static Ammo create_power() { return {enums_value_update::POWER, 0, 0, 0}; }

    enums_value_update::Ammo_type get_type() const { return type; }
    int get_rate_of_fire() const { return rate_of_fire; }
    float get_speed() const { return speed; }
    int get_damage() const { return damage; }
};

#endif //AMMO_DATA_H
