#ifndef CLIENT_H
#define CLIENT_H

#include <SDL2pp/SDL2pp.hh>

#include "../../src/common/library/socket.h"
#include "eventlistener.h"
#include "gui/gui_loop.h"
#include "gui/render.h"
#include "updater.h"

class Client {
   private:
    Socket socket;
    SDL2pp::Window& window;
    GuiLoop gui;
    EventListener eventListener;
    Updater updater;
    Event any_thread_ended;

   public:
    explicit Client(Socket socket, SDL2pp::Window& window, uint16_t player_id,
                    std::string map_name);

    void exec();

    ~Client();
};

#endif  // CLIENT_H
