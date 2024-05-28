#ifndef CLIENT_MONITOR_H
#define CLIENT_MONITOR_H

#include <vector>

#include "client.h"

class Client_Monitor {
private:
    static std::vector<Client*> list;

public:
    static void add(Client* client);
    static Client* get(int i);
    static void sendAll(std::vector<Update> updates);
    static std::vector<Client*> getAll() { return list; }
};


#endif  // CLIENT_MONITOR_H
