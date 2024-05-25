#include "eventlistener.h"

EventListener::EventListener(Socket socket) {
    protocol(std::move(socket));
}

void EventListener::listen() {
    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {//  --> evaluar como hacer el press key
                ActionType action = this->mapper.map_key_event(event);
                if(action== STOP_GAME ){
                    running= false;
                }
                protocol.send_action(action);
            }
        }
    }
}
