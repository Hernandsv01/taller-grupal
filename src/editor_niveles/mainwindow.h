#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <editormapa.h>
#include <renderizadomapa.h>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <list>

#include "../common/map/map.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   private slots:
    void on_changeBackgroundButton_clicked();

    void on_saveMapButton_clicked();

    void on_loadMapEditorButton_clicked();

    void on_goBackButton_clicked();

   private:
    Ui::MainWindow *ui;
    Map map;

    QStandardItemModel tiles;

    // El editor debe crearse despues del mapa, porque depende del tamaño del
    // mapa
    MapEditor editor;

    std::list<IdTexture> avaible_background_texture_ids;

    void changed_selected_tile(const QModelIndex &new_seleccion,
                               const QModelIndex &old_selection);
};
#endif  // MAINWINDOW_H
