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

    // MAGIA NEGRA: Si no agrego los parametros de argc y argv,
    //  la linea de mainWindow.show() tira segfault.
    //  Los parametros no se usan para absolutamente nada.
    void ejecutar(int argc, char* argv[]) {
        mainWindow.show();
        application.exec();
    }

    ~GuiLobby() {}
};

#endif