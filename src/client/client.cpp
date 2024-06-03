#include "client.h"

Client::Client(Socket&& socket, SDL2pp::Window& window)
    : socket(socket),
      window(window),
      gui(),
      eventListener(window, socket),
      updater(socket) {}

void Client::exec() {
    gui.start();
    eventListener.start();
    updater.start();
}
