#ifndef CLIENT_EVENTLISTENER_H
#define CLIENT_EVENTLISTENER_H

#include "../common/common_thread.h"
#include "../common/common_socket.h"
#include "client_protocol.h"
#include "client_mapper.h"

//Tiene que escuchar los eventos del teclado
//mapear la tecla presionada a una accion
//y mandar la accion al server a traves del socket

class EventListener : public Thread{
private:
    ClientProtocol protocol;
    Mapper mapper;
public:
    static EventListener(Socket socket);

    void listen();

};

#endif //CLIENT_EVENTLISTENER_H
