#ifndef MAPPER_H
#define MAPPER_H

#include "ActionType.h"
#include <SDL2/SDL.h>

struct Mapper{
    Mapper();
    ActionType map_key_event();
};

#endif //MAPPER_H
