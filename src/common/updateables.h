#ifndef UPDATEABLES_H
#define UPDATEABLES_H

#include <cstdint>

enum Updateables : std::uint8_t {
    HEALTH = 0x00,
    POSITION_X = 0x01,
    POSITION_Y = 0x02,
    BULLETS = 0x03
};

#endif  // UPDATEABLES_H
