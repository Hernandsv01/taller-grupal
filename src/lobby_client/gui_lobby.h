#ifndef GUI_LOBBY_H
#define GUI_LOBBY_H

#include <QApplication>
#include <utility>

#include "lobby.h"
#include "mainwindow.h"

class GuiLobby {
    // Como application recibe argc como una referencia a un int, tengo que
    // guardarlo dentro de la clase GuiLobby, para que cuando application
    // intente usar el parametro, todavía siga existiendo.
    int argc_gui;
    QApplication application;
    Lobby lobby;
    MainWindow mainWindow;

   public:
    GuiLobby(int argc, char* argv[])
        : argc_gui(argc), application(argc_gui, argv), mainWindow(lobby) {}

    void execute() {
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