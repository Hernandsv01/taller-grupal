#ifndef RENDERIZADOMAPA_H
#define RENDERIZADOMAPA_H

#include <QPaintEvent>
#include <QPainter>
#include <QStandardItemModel>
#include <QWidget>
#include <optional>

#include "../common/map/map.h"

class MapRenderer : public QWidget {
    Q_OBJECT

   protected:
    std::optional<Map*> map;
    uint x_limit = 0;
    uint y_limit = 0;
    uint tile_size = 64;
    QStandardItemModel* tiles;
    QPoint camera_reference = QPoint(0, 0);
    QPoint mouse_clicked_reference = QPoint(0, 0);
    bool moving_camera = false;
    std::optional<QImage> background;

   private:
   public:
    // explicit MapRenderer(QWidget* parent = nullptr, Map& map);
    explicit MapRenderer(QWidget* parent = nullptr);
    virtual ~MapRenderer();

    void paintEvent(QPaintEvent* event);
    void addTileModel(QStandardItemModel* newTiles);
    void setMap(Map* map);

    void drawGrid(QPainter& painter);
    void drawBackground(QPainter& painter);

    void set_camera_reference(QPoint reference);
    void modify_camera_reference(QPoint delta);

    virtual void setBackground(QImage image);

   protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void checkMapAvaible();

   signals:
};

#endif  // RENDERIZADOMAPA_H
