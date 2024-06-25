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
    int max_ammo;
    Ammo(enums_value_update::Ammo_type type, int rate_of_fire, float speed, int damage, int max_ammo) : type(type), rate_of_fire(rate_of_fire), speed(speed), damage(damage), max_ammo(max_ammo) {}

public:
    Ammo() = default;
    static Ammo create_normal() { return {enums_value_update::NORMAL, Config::get_normal_bpm(), Config::get_normal_speed(), Config::get_normal_damage(), Config::get_normal_max_ammo()}; }
    static Ammo create_light() { return {enums_value_update::LIGHT, Config::get_light_bpm(), Config::get_light_speed(), Config::get_light_damage(), Config::get_light_max_ammo()}; }
    static Ammo create_heavy() { return {enums_value_update::LIGHT, Config::get_heavy_bpm(), Config::get_heavy_speed(), Config::get_heavy_damage(), Config::get_heavy_max_ammo()}; }
    static Ammo create_power() { return {enums_value_update::LIGHT, Config::get_power_bpm(), Config::get_power_speed(), Config::get_power_damage(), Config::get_power_max_ammo()}; }

    enums_value_update::Ammo_type get_type() const { return type; }
    int get_rate_of_fire() const { return rate_of_fire; }
    float get_speed() const { return speed; }
    int get_damage() const { return damage; }
    int get_max_ammo() const { return max_ammo; }
};

#endif //AMMO_DATA_H
