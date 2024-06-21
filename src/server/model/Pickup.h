#ifndef PICKUP_H
#define PICKUP_H

#include "./constants/pickup_type.h"
#define PICKUP_W 1
#define PICKUP_H 1
#define PICKUP_VEL 0 
#define GRAVITY 0

class Pickup : public Dynamic_entity {
private:
    Pickup_type type;
    int value;
public:
    Pickup(int id, float x_spawn, float y_spawn, Pickup_type type, int value):  type(type), value(value), 
    Dynamic_entity(id, x_spawn, y_spawn, PICKUP_W, PICKUP_H, PICKUP_VEL, PICKUP_VEL, GRAVITY, false, 0, true, 0, true, true){};
                                                                                     // ...is_damageable, damage_on_contact, is_item,health, is_active,looking_right                        
    std::vector<Update::Update_new> tick(const Map& map, std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool) override {}
    Pickup_type getType() { return type; }
    int getValue() { return value; }
};

#endif //PICKUP_H
