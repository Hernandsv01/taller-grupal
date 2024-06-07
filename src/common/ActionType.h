#ifndef ACTIONTYPE_H
#define ACTIONTYPE_H

#include <sstream>

//Enum de acciones, cada accion tiene asignado un valor que es comun para el cliente y el servidor.
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

#endif //ACTIONTYPE_H
