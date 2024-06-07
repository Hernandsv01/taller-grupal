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
    uint x_limit = 1000;
    uint y_limit = 1000;
    std::vector<std::vector<Tile>> level;
    uint tile_size = 64;
    QStandardItemModel* tiles;

   public:
    explicit MapRenderer(QWidget* parent = nullptr);
    ~MapRenderer();

    void addTiles(QStandardItem* tiles);

    void paintEvent(QPaintEvent* event);
    void addTileModel(QStandardItemModel* newTiles);

   signals:
};

#endif  // RENDERIZADOMAPA_H
