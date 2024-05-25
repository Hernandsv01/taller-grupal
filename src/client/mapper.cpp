#include "mapper.h"

Mapper::Mapper() {
}

ActionType Mapper::map_key_event(const SDL_EVENT& event){
    ActionType action;
    if(event.type == SDL_KEYDOWN){
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                action = RUN_RIGHT;
                break;
            case SDLK_LEFT:
                action = RUN_LEFT;
                break;
            case SDLK_q:
                action = STOP_GAME;
                break;
            default:
                action = NULL_ACTION;
        }
    }
    return action;
}
