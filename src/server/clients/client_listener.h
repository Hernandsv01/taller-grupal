#ifndef SERVER_CLIENT_LISTENER_H
#define SERVER_CLIENT_LISTENER_H

#include <utility>

#include "client_monitor.h"
#include "server_client.h"
#include "socket.h"
#include "thread.h"

#define SHUTDOWN_MODE 2

class Client_listener : public Thread {
   private:
    Socket skt_listener;
    bool is_running;

   public:
    explicit Client_listener(char* port)
        : skt_listener(port), is_running(true) {}

    void run() override;
    void kill();
};

#endif  // SERVER_CLIENT_LISTENER_H
