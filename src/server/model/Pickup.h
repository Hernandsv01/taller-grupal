#ifndef PICKUP_H
#define PICKUP_H

#include "pickup_type.h"

class Pickup : public Dynamic_entity {
private:
    Pickup_type type;
    int value;
public:
    Pickup_type getType() { return type; }
    int getValue() { return value; }
};

#endif //PICKUP_H
