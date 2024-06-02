#include "client_monitor.h"

std::vector<Client*> Client_Monitor::list;

void Client_Monitor::add(Client* client) {
    list.push_back(client);
}

Client* Client_Monitor::get(int i) {
    return list[i];
}

void Client_Monitor::sendAll(std::vector<Update> updates) {
    for (auto & i : list) {
        i->getSender().addToQueue(updates);
    }
}
