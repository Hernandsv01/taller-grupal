#include "matchendedgui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MatchEndedGui w;
    w.show();
    return a.exec();
}
