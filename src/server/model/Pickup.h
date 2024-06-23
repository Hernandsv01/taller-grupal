#ifndef PICKUP_H
#define PICKUP_H

#include "./constants/pickup_type.h"
#include "./loader/config.h"

#define PICKUP_W 1
#define PICKUP_H 1
#define PICKUP_VEL 0 
#define GRAVITY 0

class Pickup : public Dynamic_entity {
private:
    Update::EntitySubtype subtype;
    Pickup_type type; 
    int value;
public:
    Pickup(int id, float x_spawn, float y_spawn, Update::EntitySubtype subtype):  subtype(subtype), 
    Dynamic_entity(id, x_spawn, y_spawn, PICKUP_W, PICKUP_H, PICKUP_VEL, PICKUP_VEL, GRAVITY, false, 0, true, 0, true, true){
        
        switch (subtype) {
        case Update::EntitySubtype::Coin:
            value = Config::get_pickup_coin();
            break;
        case Update::EntitySubtype::Carrot:
            value = Config::get_pickup_carrot();
            break;
        case Update::EntitySubtype::Light:
            value = Config::get_pickup_light();
            break;
        case Update::EntitySubtype::Heavy:
            value = Config::get_pickup_heavy();
            break;
        case Update::EntitySubtype::Power:
            value = Config::get_pickup_power();
            break;
        default:
            break;
        }
    };
                                                                                     // ...is_damageable, damage_on_contact, is_item,health, is_active,looking_right                        
    std::vector<Update::Update_new> tick(const Map& map, std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool, int& next_id) override {}
    Update::EntitySubtype get_subtype() { return subtype; }
    int getValue() { return value; }
    Pickup_type getType(){return type;}

};

#endif //PICKUP_H
