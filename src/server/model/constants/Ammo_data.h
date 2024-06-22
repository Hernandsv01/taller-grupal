#ifndef AMMO_DATA_H
#define AMMO_DATA_H

#include "Update.h"
#include "../loader/config.h"

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
    static Ammo create_normal() { return {enums_value_update::NORMAL, Config::get_normal_bpm(), Config::get_normal_speed(), Config::get_normal_damage()}; }
    static Ammo create_light() { return {enums_value_update::LIGHT, Config::get_light_bpm(), Config::get_light_speed(), Config::get_light_damage()}; }
    static Ammo create_heavy() { return {enums_value_update::LIGHT, Config::get_heavy_bpm(), Config::get_heavy_speed(), Config::get_heavy_damage()}; }
    static Ammo create_power() { return {enums_value_update::LIGHT, Config::get_power_bpm(), Config::get_power_speed(), Config::get_power_damage()}; }

    enums_value_update::Ammo_type get_type() const { return type; }
    int get_rate_of_fire() const { return rate_of_fire; }
    float get_speed() const { return speed; }
    int get_damage() const { return damage; }
};

#endif //AMMO_DATA_H
