#include "eventlistener.h"

EventListener::EventListener(SDL2pp::Window& window, Socket& socket)
    : Thread("EventListener cliente"), window(window), protocol(socket) {}

// hay codigo comentado que utilizamos para debuggear
void EventListener::run() {
    SDL_Event event;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    while (_keep_running) {
        bool success = SDL_WaitEvent(&event);

        if (!success || event.type == SDL_QUIT) {
            _keep_running = false;
        } else if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) &&
                   event.key.repeat == 0) {
            ActionType action = this->mapper.map_key_event(event);

            try {
                protocol.send_action(action);
            } catch (const ClosedConnectionError& e) {
                _keep_running = false;
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
