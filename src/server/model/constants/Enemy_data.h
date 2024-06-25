#ifndef ENEMY_DATA_H
#define ENEMY_DATA_H

#include "Update.h"
#include "Enemy.h"
#include "../loader/config.h"

class Enemy_data {
private:
    float speed;
    int damage;
    int health;
    Enemy_data(float speed, int damage, int health) : speed(speed), damage(damage), health(health) {}
    static Enemy_data create_normal() { return {Config::get_shooter_speed(), Config::get_shooter_damage(), Config::get_shooter_life()}; }
    static Enemy_data create_tank() { return {Config::get_crawler_speed(), Config::get_crawler_damage(), Config::get_crawler_life()}; }
    static Enemy_data create_speedy() { return {Config::get_flyer_speed(), Config::get_flyer_damage(), Config::get_flyer_life()}; }

public:
    Enemy_data() = default;

    static Enemy_data get_config(Update::EntitySubtype type) {
        switch (type) {
            case Update::Enemy1:
            default:
                return create_normal();
            case Update::Enemy2:
                return create_tank();
            case Update::Enemy3:
                return create_speedy();
        }
    }

    float get_speed() const { return speed; }
    int get_damage() const { return damage; }
    int get_health() const { return health; }
};

#endif //ENEMY_DATA_H
