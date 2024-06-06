#include "client.h"

#include <iostream>

Client::Client(Socket socket, SDL2pp::Window& window)
    : socket(std::move(socket)),
      window(window),
      gui(window),
      eventListener(window, this->socket),
      updater(this->socket),
      any_thread_ended() {}

// Ejecuta el cliente. Esta fucion bloquea hasta que el cliente termine.
void Client::exec() {
    // Agrego el evento de si algun thread terminó.
    updater.add_thread_ended_event(&any_thread_ended);
    eventListener.add_thread_ended_event(&any_thread_ended);
    gui.add_thread_ended_event(&any_thread_ended);

    // Inicio los threads
    updater.start();
    eventListener.start();

    gui.start();

    // Espero a que algun thread termine.
    any_thread_ended.wait();
}

Client::~Client() {
    eventListener.stop();
    updater.stop();
    gui.stop();

    eventListener.join();
    updater.join();
    gui.join();
}
