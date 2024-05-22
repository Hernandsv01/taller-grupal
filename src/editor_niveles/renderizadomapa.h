#ifndef RENDERIZADOMAPA_H
#define RENDERIZADOMAPA_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QStandardItemModel>

enum Tile {
    air,
    dirt,
    stone,
    water
};

Q_DECLARE_METATYPE(Tile)




class RenderizadoMapa : public QWidget
{
    Q_OBJECT

protected:
    uint x_limit = 1000;
    uint y_limit = 1000;
    std::vector<std::vector<Tile>> level;
    uint tile_size = 64;
    QStandardItemModel* tiles;


public:
    explicit RenderizadoMapa(QWidget* parent = nullptr);
    ~RenderizadoMapa();

    void agregarTiles(QStandardItem* tiles);

    void paintEvent(QPaintEvent* event);
    void add_tiles(QStandardItemModel* newTiles);

signals:

};

#endif // RENDERIZADOMAPA_H
