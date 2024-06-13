#ifndef GUI_LOBBY_H
#define GUI_LOBBY_H

#include <QApplication>
#include <iostream>

#include "lobby.h"
#include "mainwindow.h"

class GuiLobby {
    QApplication application;
    Lobby lobby;
    MainWindow mainWindow;

   public:
    GuiLobby(int argc, char* argv[])
        : application(argc, argv), mainWindow(lobby) {}

    void ejecutar(int argc, char* argv[]) {
        mainWindow.show();
        application.exec();
    }

    ~GuiLobby() {}
};

#endif