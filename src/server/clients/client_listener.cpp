#include "client_listener.h"

void Client_listener::run() {
    while (is_running) {
        try {
            Socket skt = skt_listener.accept();
            Client_Monitor::add(new Server_Client(std::move(skt)));
        } catch (std::exception& e) {
            is_running = false;
        }
    }
}

void Client_listener::kill() {
    is_running = false;
    skt_listener.shutdown(SHUTDOWN_MODE);
    skt_listener.close();
}
