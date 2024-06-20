#include <QApplication>
#include <iostream>
#include <vector>

#include "matchendedgui.h"

int main(int argc, char *argv[]) {
    std::vector<PlayerScore> scores;

    std::vector<std::string> names = {"Jazz", "Spaz", "Lori"};

    for (int i = 0; i < 30; i++) {
        PlayerScore score;
        uint hash = std::hash<std::string>{}(std::to_string(i));

        score.name_bunny = names[i % 3];
        score.id_player = i;
        score.score = hash % 100;

        if (i == 7) {
            score.is_current_player = true;
        } else
            score.is_current_player = false;
        scores.push_back(score);
    }

    QApplication a(argc, argv);
    MatchEndedGui w(scores);
    w.show();
    return a.exec();
}
