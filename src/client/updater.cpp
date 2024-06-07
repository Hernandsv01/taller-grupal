#include "updater.h"

Updater::Updater(Socket& socket) : protocol(socket) {}

/*
 * Recive la informacion actualizada del lado del server a traves del socket.
 * y envia la informacion actualizada a la updated queue, que luego es leida por el gui.
 */
void Updater::run() {
    while (this->keep_running()) {
        std::vector<Update> updated_info = protocol.receive_ticks();
        Update_queue::push(updated_info);
    }
}

void Updater::stop_custom() { protocol.kill(); }
