#include <QApplication>
#include <iostream>
#include <vector>

#include "matchendedgui.h"

int main(int argc, char *argv[]) {
    std::vector<PlayerScore> scores;

    for (int i = 0; i < 30; i++) {
        PlayerScore score;
        score.name_bunny = "Bunny";
        score.id_player = i;
        score.score = std::hash<std::string>{}(std::to_string(i)) % 100;
        if (i == 7) {
            score.is_current_player = true;
            score.score = 100;
        } else
            score.is_current_player = false;
        scores.push_back(score);
    }

    QApplication a(argc, argv);
    MatchEndedGui w(scores);
    w.show();
    return a.exec();
}
