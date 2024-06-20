#ifndef PICKUP_H
#define PICKUP_H

#include "pickup_type.h"

class Pickup : public Dynamic_entity {
private:
    Pickup_type type;
    int value;
public:
    std::vector<Update::Update_new> tick(const Map& map, std::vector<std::unique_ptr<Dynamic_entity>>& entity_pool) override {}
    Pickup_type getType() { return type; }
    int getValue() { return value; }
};

#endif //PICKUP_H
