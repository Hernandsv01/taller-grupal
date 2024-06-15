#ifndef RENDERIZADOMAPA_H
#define RENDERIZADOMAPA_H

#include <QPaintEvent>
#include <QPainter>
#include <QStandardItemModel>
#include <QWidget>

enum Tile { air, dirt, stone, water };

Q_DECLARE_METATYPE(Tile)

class MapRenderer : public QWidget {
    Q_OBJECT

   protected:
    uint x_limit = 255;
    uint y_limit = 255;
    std::vector<std::vector<Tile>> level;
    uint tile_size = 64;
    QStandardItemModel* tiles;
    QPoint camera_reference = QPoint(0, 0);
    QPoint mouse_clicked_reference = QPoint(0, 0);
    bool moving_camera = false;

   public:
    explicit MapRenderer(QWidget* parent = nullptr);
    ~MapRenderer();

    void paintEvent(QPaintEvent* event);
    void addTileModel(QStandardItemModel* newTiles);

    void drawGrid(QPainter& painter);

    void set_camera_reference(QPoint reference);
    void modify_camera_reference(QPoint delta);

   protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

   signals:
};

#endif  // RENDERIZADOMAPA_H
