#ifndef UPDATER_H
#define UPDATER_H

#include <vector>

#include "../common/library/socket.h"
#include "../common/library/thread.h"
#include "protocol.h"
#include "update_queue.h"

// Escucha el server y actualiza los states de las entidades del juego.
//
class Updater : public Thread {
   private:
    ClientProtocol protocol;

    std::string text_description() override { return "Updater Cliente"; }

   public:
    explicit Updater(Socket& socket);

    void run() override;

    void stop_custom() override;
};

#endif  // UPDATER_H
