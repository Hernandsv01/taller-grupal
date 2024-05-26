#include "updater.h"

Updater::Updater(Socket& socket) : protocol(socket){}

void Updater::run() {
    while(is_running){
        //protocol.receive();
        //mappea
        //arma paquete de actualizacion
        //meter en q de renderer
    }
}

void Updater::stop() {
    this->is_running = false;
    protocol.kill();
}
