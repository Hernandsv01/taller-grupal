#ifndef MATCHENDEDGUI_H
#define MATCHENDEDGUI_H

#include <QMainWindow>

struct PlayerScore {
    std::string name_bunny;
    uint16_t id_player;
    int score;
    bool is_current_player;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MatchEndedGui;
}
QT_END_NAMESPACE

class MatchEndedGui : public QMainWindow {
    Q_OBJECT

   public:
    MatchEndedGui(std::vector<PlayerScore> scores, QWidget *parent = nullptr);
    ~MatchEndedGui();

   private:
    Ui::MatchEndedGui *ui;
    std::vector<PlayerScore> scores;

    void orderScores(std::vector<PlayerScore> &scores);
    void addScoresToGui(std::vector<PlayerScore> scores);
};
#endif  // MATCHENDEDGUI_H
