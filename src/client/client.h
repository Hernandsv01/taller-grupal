#ifndef CLIENT_H
#define CLIENT_H

#include "../../src/common/library/socket.h"
#include "eventlistener.h"
#include "updater.h"
#include "gui/render.h"
#include "gui/gui_loop.h"

class Client{
private:
    Socket& socket;
    GuiLoop gui;
    EventListener eventListener;
    Updater updater;

public:
    explicit Client(Socket&& socket);

    void exec();

};

#endif //CLIENT_H
