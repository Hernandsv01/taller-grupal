#ifndef PICKUP_TYPE_H
#define PICKUP_TYPE_H

#include "../../../common/Update.h"

enum Pickup_type {
    COIN = Update::EntitySubtype::Coin,
    CARROT = Update::EntitySubtype::Carrot,
    LIGHT_AMMO = Update::EntitySubtype::Light,
    HEAVY_AMMO = Update::EntitySubtype::Heavy,
    POWER_AMMO = Update::EntitySubtype::Power
};

#endif //PICKUP_TYPE_H
