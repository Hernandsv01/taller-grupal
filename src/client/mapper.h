#ifndef MAPPER_H
#define MAPPER_H

#include "../common/dtos.h"
#include <SDL2/SDL.h>

struct Mapper{
    Mapper();
    ActionType map_key_event(const SDL_Event& event);
};

#endif //MAPPER_H
