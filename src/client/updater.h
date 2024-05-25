#ifndef CLIENT_UPDATER_H
#define CLIENT_UPDATER_H

#include "../common/common_thread.h"
#include "../common/common_socket.h"
#include "client_protocol.h"
//Escucha el server y actualiza los estados de las entidades del juego.
//
class Updater : public Thread{
private:
    ClientProtocol protocol;
public:
    static Updater(Socket socket);
};

#endif //CLIENT_UPDATER_H
