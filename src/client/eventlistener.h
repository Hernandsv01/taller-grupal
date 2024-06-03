#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include <SDL2pp/SDL2pp.hh>

#include "../common/library/socket.h"
#include "../common/library/thread.h"
#include "mapper.h"
#include "protocol.h"

// Tiene que escuchar los eventos del teclado
// mapear la tecla presionada a una accion
// y mandar la accion al server a traves del socket

class EventListener : public Thread {
   private:
    SDL2pp::Window& window;
    ClientProtocol protocol;
    Mapper mapper;

    void process_event(const SDL_Event& event, bool& running);

   public:
    EventListener(SDL2pp::Window& window, Socket& socket);

    // loop que escucha los eventos del teclado
    void run();
};

#endif  // EVENTLISTENER_H
