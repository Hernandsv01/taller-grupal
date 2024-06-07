#include "updater.h"

Updater::Updater(Socket& socket) : protocol(socket) {}

void Updater::run() {
    while (this->keep_running()) {
        std::vector<Update> updated_info = protocol.receive_ticks();
        Update_queue::push(updated_info);
    }
}

void Updater::stop_custom() { protocol.kill(); }
