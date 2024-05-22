#include "renderizadomapa.h"

#include <iostream>
#include <QVector>

void dibujar_cuadricula(QPainter& painter, int tile_size, int width, int height) {
    QVector<QLine> lineas;

    for(int x = 0; x <= width ; x+=tile_size) {
        QLine linea(x, 0, x, height);
        lineas.push_back(linea);
    }

    for(int y = 0; y <= height ; y+=tile_size) {
        QLine linea(0, y, width, y);
        lineas.push_back(linea);
    }

    painter.drawLines(lineas);
}

QString get_tile_name(Tile unTile) {
    switch(unTile) {
    case Tile::dirt: return "dirt.png";
    case Tile::stone: return "stone.png";
    case Tile::water: return "water.png";
    case Tile::air:
        defaule:
        qDebug("Case tile air: Deberia ser un error");
        return "ERROR?";
    }

    return "ERROR?";
}

RenderizadoMapa::RenderizadoMapa(QWidget* parent)
    : QWidget{parent}
{
    this->tile_size = 64;

    for (int i = 0; i < y_limit; i++) {
        level.emplace_back(std::vector<Tile>(x_limit, Tile::air));
    }

    level[0][0] = Tile::dirt;

    level[5][3] = Tile::water;

    level[4][2] = Tile::stone;

}

RenderizadoMapa::~RenderizadoMapa() {
    if (tiles) delete tiles;
}

void RenderizadoMapa::add_tiles(QStandardItemModel* newTiles) {
    this->tiles = newTiles;
}


void RenderizadoMapa::paintEvent(QPaintEvent* event) {
    QPainter painter(this);


    int width = this->width();
    int height = this->height();

    for(uint y = 0; y < (height / tile_size); y++) {
        //En el caso de que quiera renderizar algo
        //que este fuera de lo definido por level
        if (y >= y_limit)
            break;

        for (uint x = 0; x < (width / tile_size); x++) {
            //En el caso de que quiera renderizar algo
            //que este fuera de lo definido por level
            if (x >= x_limit)
                break;

            Tile content = level[x][y];
            if (content == Tile::air)
                continue;



            QList<QStandardItem*> tile_items = tiles->findItems(get_tile_name(content));

            if (tile_items.isEmpty()) continue;

            QStandardItem* tile_item = tile_items[0];
            QVariant image_variant = tile_item->data(Qt::UserRole+2);

            QImage image = image_variant.value<QImage>();

            QRect rectangle_to_draw(x*tile_size, y*tile_size, tile_size, tile_size);

            painter.drawImage(rectangle_to_draw, image);


        }
    }

    dibujar_cuadricula(painter, this->tile_size, width, height);


}





