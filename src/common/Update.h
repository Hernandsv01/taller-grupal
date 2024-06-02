#ifndef COMMON_DTOS_H
#define COMMON_DTOS_H

#include <sstream>

struct Update{
    std::uint16_t id;
    std::uint8_t key;
    std::uint16_t value;
}__attribute__((packed));

#endif //COMMON_DTOS_H