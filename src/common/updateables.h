#ifndef UPDATEABLES_H
#define UPDATEABLES_H

#include <cstdint>

enum Updateables : std::uint8_t {
    HEALTH = 0x00,
    POINTS = 0X01,
    AMMO_TYPE = 0X02,
    BULLETS = 0X03,
    POSITION_X = 0x04,
    POSITION_Y = 0x05,
    DESTROYED = 0x06
};

#endif  // UPDATEABLES_H
