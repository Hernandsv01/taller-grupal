#ifndef CLIENT_MONITOR_H
#define CLIENT_MONITOR_H

#include <vector>

#include "server_client.h"

//TODO: ELIMINAR? creo que ya no se usa.
class Client_Monitor {
   private:
    static std::vector<Server_Client*> list;

   public:
    static void add(Server_Client* client);
    static Server_Client* get(int i);
    static void sendAll(std::vector<Update::Update_new> updates);
    static std::vector<Server_Client*> getAll() { return list; }
    ~Client_Monitor();
};

#endif  // CLIENT_MONITOR_H
