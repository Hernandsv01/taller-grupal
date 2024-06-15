#ifndef SERVER_CLIENT_LISTENER_H
#define SERVER_CLIENT_LISTENER_H

#include <utility>

#include "client_monitor.h"
#include "server_client.h"
#include "../../common/library/socket.h"
#include "../../common/library/thread.h"

#define SHUTDOWN_MODE 2

class Client_listener : public Thread {
   private:
    Socket& skt_listener;
    bool is_running;

   public:
    explicit Client_listener(Socket& socket)
        : Thread("Client_listener server"),
          skt_listener(socket),
          is_running(true) {}

    void run() override;
    void kill();
};

#endif  // SERVER_CLIENT_LISTENER_H
