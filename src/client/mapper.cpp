#include "mapper.h"

Mapper::Mapper() {}

ActionType Mapper::map_key_event(const SDL_EVENT& event){
    ActionType action = NULL_ACTION;
    if(event.type == SDL_KEYDOWN){
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                action = RUN_RIGHT;
                break;
            case SDLK_LEFT:
                action = RUN_LEFT;
                break;
            case SDLK_SPACE:
                action = JUMP;
                break;
            default:
                action = NULL_ACTION;
        }
    } else if (event.type == SDL_KEYUP){
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                action = STOP_RUN_RIGHT;
                break;
            case SDLK_LEFT:
                action = STOP_RUN_LEFT;
                break;
            default:
                action = NULL_ACTION;
        }
    }
    return action;
}
