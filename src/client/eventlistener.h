#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include <SDL2/SDL.h>
#include "../common/library/thread.h"
#include "../common/library/socket.h"
#include "protocol.h"
#include "mapper.h"

//Tiene que escuchar los eventos del teclado
//mapear la tecla presionada a una accion
//y mandar la accion al server a traves del socket

class EventListener : public Thread{
private:
    SDL_Window& window;
    ClientProtocol protocol;
    Mapper mapper;

    void process_event(const SDL_Event& event, bool& running);

public:
    explicit EventListener(SDL_Window& window, Socket& socket);

    //loop que escucha los eventos del teclado
    void run();

};

#endif //EVENTLISTENER_H
