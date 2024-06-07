#include "mainwindow.h"

#include <QApplication>
#include "lobby.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lobby lobby;
    MainWindow w(lobby);
    w.show();
    return a.exec();
}
