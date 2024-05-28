#include "updater.h"

Updater::Updater(Socket& socket) : protocol(socket){}



void Updater::run() {
    while(is_running){
        std::vector<Update> updated_info = protocol.receive_ticks();
        Update_queue::push(updated_info);
    }
}

void Updater::stop() {
    this->is_running = false;
    protocol.kill();
}
