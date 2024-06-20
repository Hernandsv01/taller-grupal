#include "client.h"

#include <iostream>

Client::Client(Socket socket, SDL2pp::Window& window, uint16_t player_id,
               std::string map_name)
    : socket(std::move(socket)),
      window(window),
      gui(window, player_id, map_name),
      eventListener(window, this->socket),
      updater(this->socket),
      any_thread_ended() {}

/*
 * Ejecuta los hilos de gui, el updater y el eventListener.
 * Esta funcion bloquea hasta que alguno de los hilos termine.
 */
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
    std::cout << "Empiezo a esperar" << std::endl;
    any_thread_ended.wait();
    std::cout << "Algun thread ya paró" << std::endl;

    stopThreads();

    window.Hide();
}

bool Client::matchEnded() { return gui.hasMatchEnded(); }

void Client::stopThreads() {
    eventListener.stop();
    updater.stop();
    gui.stop();

    eventListener.join();
    updater.join();
    gui.join();
}

Client::~Client() {}
