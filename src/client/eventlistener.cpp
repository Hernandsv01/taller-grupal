#include "eventlistener.h"

EventListener::EventListener(SDL2pp::Window& window, Socket& socket)
    : Thread("EventListener cliente"), window(window), protocol(socket) {}

// hay codigo comentado que utilizamos para debuggear
void EventListener::run() {
    SDL_Event event;

    uint last_event_type = SDL_QUIT;

    while (_keep_running) {
        bool success = SDL_WaitEvent(&event);

        if (event.type != last_event_type) {
            std::cout << "Evento: " << event.type << std::endl;
            last_event_type = event.type;
        }

        if (!success || event.type == SDL_QUIT) {
            _keep_running = false;
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

            try {
                protocol.send_action(action);
            } catch (const ClosedConnectionError& e) {
                _keep_running = false;
                // TODO: Habria que implementar algo para manejar el error.
            }
        }
    }
}

void EventListener::stop_custom() {
    _keep_running = false;
    SDL_Event quit_event;
    quit_event.type = SDL_QUIT;
    SDL_PushEvent(&quit_event);
}
