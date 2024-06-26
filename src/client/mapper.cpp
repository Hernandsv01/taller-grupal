#include "mapper.h"

Mapper::Mapper() {}

/*
 * Mapea las teclas presionadas a la accion que corresponde.
 * Por ahora aceptan las flechas, la barra espaciadora, el return y la tecla m.
 * Falta definir que teclas se utilizaran y si ademas vamos a permitir el movimiento con awsd.
 */
ActionType Mapper::map_key_event(const SDL_Event& event){
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
            case SDLK_RETURN:
                action = SHOOT;
                break;
            case SDLK_n:
                action = SWITCH_GUN;
                break;
            case SDLK_m:
                action = SPECIAL;
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
            case SDLK_RETURN:
                action = STOP_SHOOT;
                break;
            default:
                action = NULL_ACTION;
        }
    }
    return action;
}
