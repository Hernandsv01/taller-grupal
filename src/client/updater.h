#ifndef UPDATER_H
#define UPDATER_H

#include "../common/library/thread.h"
#include "../common/library/socket.h"
#include "protocol.h"
#include "update_queue.h"
#include <vector>

//Escucha el server y actualiza los estados de las entidades del juego.
//
class Updater : public Thread{
private:
    ClientProtocol protocol;
    bool is_running = true;
public:
    explicit Updater(Socket& socket);

    //envia la informacion actualizada al
    void run();

    void stop();
};

#endif //UPDATER_H
