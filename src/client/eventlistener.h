#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include "../common/thread.h"
#include "../common/socket.h"
#include "protocol.h"
#include "mapper.h"

//Tiene que escuchar los eventos del teclado
//mapear la tecla presionada a una accion
//y mandar la accion al server a traves del socket

class EventListener : public Thread{
private:
    ClientProtocol protocol;
    Mapper mapper;

    void process_event(const SDL_Event& event, bool& running) {
public:
    static EventListener(Socket socket);

    //loop que escucha los eventos del teclado
    void listen();

};

#endif //EVENTLISTENER_H
