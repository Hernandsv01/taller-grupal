#ifndef COMMON_DTOS_H
#define COMMON_DTOS_H

#include <sstream>

enum ActionType: std::uint8_t {
    NULL_ACTION = 0x00,

    JUMP = 0x01,
    RUN_LEFT = 0x02,
    RUN_RIGHT = 0x03,

    SHOOT = 0x10,
    SPECIAL = 0x11, //esta accion es diferente para cada personaje (pero se evalua en el server).

    STOP_RUN_RIGHT = 0x20,
    STOP_RUN_LEFT = 0x21
};

struct ActionMessage{
    std::uint8_t action;
}__attribute__((packed));

//msj que recibe desde el server para mapear a la entidad.
struct Update{
    std::uint8_t id;
    std::uint8_t key;
    std::uint8_t value;
}__attribute__((packed));

#endif //COMMON_DTOS_H