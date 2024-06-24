#include <QApplication>

#include "matchendedgui.h"

class MatchEnded {
    QApplication app;
    MatchEndedGui* window = nullptr;

   public:
    MatchEnded(int argc, char* argv[]) : app(argc, argv) {}
    void showWithScores(std::vector<PlayerScore> scores) {
        window = new MatchEndedGui(scores);
        window->show();
        app.exec();
    }

    ~MatchEnded() {
        if (window) {
            delete window;
        }
    }
};