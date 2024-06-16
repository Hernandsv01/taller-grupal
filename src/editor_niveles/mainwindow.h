#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <editormapa.h>
#include <renderizadomapa.h>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>

#include "../common/map/map.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    // MainWindow(const char *map_path, QWidget *parent = nullptr);
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private slots:
    void on_changeBackgroundButton_clicked();

   private:
    Ui::MainWindow *ui;
    MapEditor editor;
    QVector<QImage> backgrounds;
    Map *map;
};
#endif  // MAINWINDOW_H
