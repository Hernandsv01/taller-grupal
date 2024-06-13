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

    // MAGIA NEGRA: Si no agrego los parametros de argc y argv,
    //  la linea de mainWindow.show() tira segfault.
    //  Los parametros no se usan para absolutamente nada.
    void execute(int argc, char* argv[]) {
        mainWindow.show();
        application.exec();
    }

    bool isConnectedToMatch() { return lobby.isConnectedToMatch(); }

    std::pair<uint16_t, std::string> getPlayerIdAndMapName() {
        return lobby.getPlayerIdAndMapName();
    }

    Socket getMatchConnection() { return lobby.getMatchConnection(); }

    ~GuiLobby() {}
};

#endif