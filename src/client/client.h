#ifndef CLIENT_H
#define CLIENT_H

#include "../../src/common/library/socket.h"
#include "eventlistener.h"
#include "gui/gui_loop.h"
#include "gui/render.h"
#include "updater.h"

class Client {
   private:
    Socket& socket;
    Window& window;
    GuiLoop gui;
    EventListener eventListener;
    Updater updater;

   public:
    explicit Client(Socket&& socket, Window& window);

    void exec();
};

#endif  // CLIENT_H
