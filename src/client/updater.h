#ifndef UPDATER_H
#define UPDATER_H

#include "../common/thread.h"
#include "../common/socket.h"
#include "protocol.h"
//Escucha el server y actualiza los estados de las entidades del juego.
//
class Updater : public Thread{
private:
    ClientProtocol protocol;
public:
    static Updater(Socket socket);

    //envia la informacion actualizada al
    void update();
};

#endif //UPDATER_H
