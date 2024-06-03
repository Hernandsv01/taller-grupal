#include "client.h"

#include <iostream>

Client::Client(Socket socket, SDL2pp::Window& window)
    : socket(std::move(socket)),
      window(window),
      //   gui(),
      eventListener(window, this->socket),
      updater(this->socket) {}

void Client::exec() {
    // gui.start();

    updater.start();
    eventListener.start();
}

Client::~Client() {
    eventListener.stop();
    updater.stop();

    eventListener.join();
    updater.join();
}
