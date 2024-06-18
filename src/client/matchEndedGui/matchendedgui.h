#ifndef MATCHENDEDGUI_H
#define MATCHENDEDGUI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MatchEndedGui; }
QT_END_NAMESPACE

class MatchEndedGui : public QMainWindow
{
    Q_OBJECT

public:
    MatchEndedGui(QWidget *parent = nullptr);
    ~MatchEndedGui();

private:
    Ui::MatchEndedGui *ui;
};
#endif // MATCHENDEDGUI_H
