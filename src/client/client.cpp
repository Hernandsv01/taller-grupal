#include "client.h"

#include <iostream>

Client::Client(Socket socket, SDL2pp::Window& window)
    : socket(std::move(socket)),
      window(window),
      gui(window),
      eventListener(window, this->socket),
      updater(this->socket) {}

/*
 * Ejecuta los hilos de gui, el updater y el eventListener.
 */
void Client::exec() {
    updater.start();
    eventListener.start();

    gui.start();
}

Client::~Client() {
    eventListener.stop();
    updater.stop();
    gui.stop();

    eventListener.join();
    updater.join();
    gui.join();
}
