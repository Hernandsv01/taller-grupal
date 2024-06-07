#include "eventlistener.h"

EventListener::EventListener(SDL2pp::Window& window, Socket& socket)
    : Thread("EventListener cliente"), window(window), protocol(socket) {}

//hay codigo comentado que utilizamos para debuggear
void EventListener::run() {
    SDL_Event event;
    bool running = true;
    while (running) {
        bool success = SDL_WaitEvent(&event);

        if (!success || event.type == SDL_QUIT) {
            running = false;
        } else if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) &&
                   event.key.repeat == 0) {
            // DEBUG:
            // const char* eventType = (event.type == SDL_KEYDOWN)
            //                             ? "Key pressed: "
            //                             : "Key released: ";
            // std::cout << eventType <<
            // SDL_GetKeyName(event.key.keysym.sym)
            //           << std::endl;

            ActionType action = this->mapper.map_key_event(event);

            protocol.send_action(action);
        }
    }
}
