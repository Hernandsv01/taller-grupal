#include "client.h"

Client::Client(Socket &&socket) : socket(socket), render(800, 600), gui(), eventListener(render.get_window(), socket), updater(socket) {
}

void Client::exec() {
    //inicializae el render
    gui.start();
    eventListener.start();
    updater.start();
}


