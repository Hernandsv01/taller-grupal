#ifndef COMMON_DTOS_H
#define COMMON_DTOS_H

#include <sstream>

//packete de actualizacion: se utiliza el id, una clave del elemento actualizabel y el valor.
struct Update {
    std::uint16_t id;
    std::uint8_t key;
    std::uint32_t value;
} __attribute__((packed));

#endif  // COMMON_DTOS_H