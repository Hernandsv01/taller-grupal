#ifndef GUI_LOBBY_H
#define GUI_LOBBY_H

#include <QApplication>
#include <utility>

#include "lobby.h"
#include "mainwindow.h"

class GuiLobby {
    QApplication application;
    Lobby lobby;
    MainWindow mainWindow;

   public:
    GuiLobby(int argc, char* argv[])
        : application(argc, argv), mainWindow(lobby) {}

    void execute() {
        // MAGIA NEGRA: por alguna razon, si llamo a mainWindow.show() sin
        // agregar la siguiente variable, el programa tira segfault.
        // Supongo que tiene que ver con alguna alineacion del stack.
        uint8_t align_stack = 0;

        mainWindow.show();
        application.exec();
    }

    bool isConnectedToMatch() { return lobby.isConnectedToMatch(); }

    std::pair<uint16_t, std::string> getPlayerIdAndMapName() {
        return lobby.getPlayerIdAndMapName();
    }

    Socket extractMatchConnection() { return lobby.extractMatchConnection(); }

    ~GuiLobby() {}
};

#endif