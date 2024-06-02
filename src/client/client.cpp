#include "client.h"

Client::Client(Socket &&socket, Window window) : socket(socket), gui(window), eventListener(window, socket), updater(socket) {
}

void Client::exec() {
    gui.start();
    eventListener.start();
    updater.start();
}


