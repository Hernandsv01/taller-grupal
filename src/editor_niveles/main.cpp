#include <QApplication>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    char* map = nullptr;

    if (argc > 1) {
        map = argv[1];
    }

    MainWindow w(map);
    w.show();
    return a.exec();
}
