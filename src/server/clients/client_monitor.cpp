#include "client_monitor.h"

std::vector<Server_Client*> Client_Monitor::list;

void Client_Monitor::add(Server_Client* client) { list.push_back(client); }

Server_Client* Client_Monitor::get(int i) { return list[i]; }

void Client_Monitor::sendAll(std::vector<Update> updates) {
    for (auto& i : list) {
        i->getSender().addToQueue(updates);
    }
}

Client_Monitor::~Client_Monitor() {
    // Agregado destructor para los puntero a erver_Client
    while (Client_Monitor::list.size() > 0) {
        delete Client_Monitor::list.back();
        Client_Monitor::list.pop_back();
    }
}
