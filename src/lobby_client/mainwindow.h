#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "lobby.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(Lobby& lobby);
    ~MainWindow();

   private:
    Lobby& lobby;
    void updateMatchesFromMatchList();
    void goToMatchSelection();
    std::vector<GameMatch> getGameMatches();
    void closedConnectionError();

   private slots:

    void on_botonConectar_clicked();

    void on_botonUnirseAPartida_clicked();

    void on_botonIrACrearPartida_clicked();

    void on_botonCrearPartida_clicked();

    void on_botonCancelarCrearPartida_clicked();

    void on_botonRecargarPartidas_clicked();

private:
    Ui::MainWindow* ui;
};
#endif  // MAINWINDOW_H
