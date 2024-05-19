#ifndef CLIENT_EVENTLISTENER_H
#define CLIENT_EVENTLISTENER_H

#include "common_thread.h"
#include "client_protocol.h"

//Tiene que escuchar los eventos del teclado
//mapear la tecla presionada a una accion
//y mandar la accion al server a traves del socket

class EventListener : public Thread{
private:
    ClientProtocol protocol;
public:

};

#endif //CLIENT_EVENTLISTENER_H
