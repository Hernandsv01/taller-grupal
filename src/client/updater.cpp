#include "updater.h"

Updater::Updater(Socket& socket)
    : Thread("Updater Cliente"), protocol(socket) {}

/*
 * Recive la informacion actualizada del lado del server a traves del socket.
 * y envia la informacion actualizada a la updated queue, que luego es leida por el gui.
 */
void Updater::run() {
    while (this->keep_running()) {
        try {
            std::vector<Update::Update_new> updated_info = protocol.receive_ticks();
            Update_queue::push(updated_info);
        } catch (const ClosedConnectionError& e) {
            break;
        }
    }
}

void Updater::stop_custom() { protocol.kill(); }
