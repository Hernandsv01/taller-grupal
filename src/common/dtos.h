#ifndef COMMON_DTOS_H
#define COMMON_DTOS_H

enum ActionType{
    JUMP = 0x01,
    SHOOT = 0x02,
    RUN_LEFT = 0x03,
    RUN_RIGHT = 0x04,
    SPECIAL = 0x05 //esta accion es diferente para cada personaje (pero se evalua en el server).
};

struct ActionMessage{
    std::uint8_t action;
}__attribute__((packed));

//msj que recibe desde el server para mapear a la entidad.
struct Update{
};

#endif //COMMON_DTOS_H
