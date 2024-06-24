#include "matchendedgui.h"

#include <QDebug>
#include <QTableWidgetItem>

#include "./ui_matchendedgui.h"

MatchEndedGui::MatchEndedGui(std::vector<PlayerScore> scores, QWidget* parent)
    : scores(scores), QMainWindow(parent), ui(new Ui::MatchEndedGui) {
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeMode::Stretch);

    ui->tableWidget->setFocusPolicy(Qt::NoFocus);

    orderScores(scores);
    addScoresToGui(scores);
}

MatchEndedGui::~MatchEndedGui() { delete ui; }

void MatchEndedGui::orderScores(std::vector<PlayerScore>& scores) {
    std::sort(scores.begin(), scores.end(),
              [](PlayerScore a, PlayerScore b) { return a.score > b.score; });
}

void MatchEndedGui::addScoresToGui(std::vector<PlayerScore> scores) {
    int count = 1;

    QColor baseColour = QMainWindow::palette().color(QPalette::Base);

    QColor gold = QColor(166, 140, 0);
    QColor silver = QColor(125, 125, 125);
    QColor copper = QColor(133, 83, 32);

    for (const auto& score : scores) {
        // QTableWidget
        ui->tableWidget->setRowCount(scores.size());
        ui->tableWidget->setColumnCount(4);

        int position = 1;
        for (const auto& score : scores) {
            QTableWidgetItem* rankItem =
                new QTableWidgetItem(QString::number(position) + QString("°"));
            QTableWidgetItem* nameItem =
                new QTableWidgetItem(QString::fromStdString(score.name_bunny));
            QTableWidgetItem* idItem =
                new QTableWidgetItem(QString::number(score.id_player));
            QTableWidgetItem* scoreItem =
                new QTableWidgetItem(QString::number(score.score));

            QColor colourToUse = baseColour;

            if (position == 1) {
                colourToUse = gold;
            } else if (position == 2) {
                colourToUse = silver;
            } else if (position == 3) {
                colourToUse = copper;
            }
            // colourToUse = player;
            if (score.is_current_player) {
                idItem->setText(idItem->text() + " (Tú)");

                // QColor text_color(180, 255, 180);

                // rankItem->setForeground(text_color);
                // nameItem->setForeground(text_color);
                // idItem->setForeground(text_color);
                // scoreItem->setForeground(text_color);

                QFont fontStyle =
                    QFont("Arial", 15, QFont::Weight::Black, true);

                rankItem->setFont(fontStyle);
                nameItem->setFont(fontStyle);
                idItem->setFont(fontStyle);
                scoreItem->setFont(fontStyle);
            }

            rankItem->setBackground(colourToUse);
            nameItem->setBackground(colourToUse);
            idItem->setBackground(colourToUse);
            scoreItem->setBackground(colourToUse);

            ui->tableWidget->setItem(position - 1, 0, rankItem);
            ui->tableWidget->setItem(position - 1, 1, nameItem);
            ui->tableWidget->setItem(position - 1, 2, idItem);
            ui->tableWidget->setItem(position - 1, 3, scoreItem);

            position++;
        }

        count++;
    }
}
