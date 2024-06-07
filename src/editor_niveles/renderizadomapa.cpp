#include "renderizadomapa.h"

#include <QVector>
#include <iostream>

void dibujar_cuadricula(QPainter& painter, int tile_size, int width,
                        int height) {
    QVector<QLine> lineas;

    // Agrego a un vector todas las lineas horizontales y verticales a dibujar
    for (int x = 0; x <= width; x += tile_size) {
        QLine linea(x, 0, x, height);
        lineas.push_back(linea);
    }

    for (int y = 0; y <= height; y += tile_size) {
        QLine linea(0, y, width, y);
        lineas.push_back(linea);
    }

    // dibujo todas las lineas
    painter.drawLines(lineas);
}

QString get_tile_name(Tile unTile) {
    // Por ahora utilizo este switch para obtener el nombre del archivo.
    // Tal vez haya una manera mejor de hacerlo?
    switch (unTile) {
        case Tile::dirt:
            return "dirt.png";
        case Tile::stone:
            return "stone.png";
        case Tile::water:
            return "water.png";
        case Tile::air:
        default:
            qDebug("Case tile air: Deberia ser un error");
            return "ERROR?";
    }

    return "ERROR?";
}

MapRenderer::MapRenderer(QWidget* parent) : QWidget{parent}, tiles{nullptr} {
    // Defino el tilesize. Esto indica cuantos pixeles ocupa cada casillero de
    // la grilla.
    this->tile_size = 64;

    // Creo una grilla de x_limit * y_limit. Inicialmente todo es aire.
    for (int i = 0; i < y_limit; i++) {
        level.emplace_back(std::vector<Tile>(x_limit, Tile::air));
    }

    // Defino algunos tiles en la grilla para testear.
    level[0][0] = Tile::dirt;

    level[5][3] = Tile::water;

    level[4][2] = Tile::stone;
}

MapRenderer::~MapRenderer() {
    if (tiles) delete tiles;
}

void MapRenderer::addTileModel(QStandardItemModel* newTiles) {
    this->tiles = newTiles;
}

void MapRenderer::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    int width = this->width();
    int height = this->height();

    // Loop para recorrer la grilla de tiles y dibujarlos
    for (uint y = 0; y <= (height / tile_size); y++) {
        // En el caso de que quiera renderizar algo
        // que este fuera de lo definido por level
        if (y >= y_limit) break;

        for (uint x = 0; x <= (width / tile_size); x++) {
            // En el caso de que quiera renderizar algo
            // que este fuera de lo definido por level
            if (x >= x_limit) break;

            // Obtengo el tipo de tile, si es aire lo omito.
            Tile content = level[x][y];
            if (content == Tile::air) continue;

            // Busco el item en la lista de tiles
            // (por ahora necesito el nombre. Tal vez hay alguna manera mejor?)
            QList<QStandardItem*> tile_items =
                tiles->findItems(get_tile_name(content));

            if (tile_items.isEmpty()) continue;

            QStandardItem* tile_item = tile_items[0];

            // Obtengo la imagen del tile
            QVariant image_variant = tile_item->data(Qt::UserRole + 2);

            QImage image = image_variant.value<QImage>();

            // Dibujo el tile en la posicion correcta.
            QRect rectangle_to_draw(x * tile_size, y * tile_size, tile_size,
                                    tile_size);

            painter.drawImage(rectangle_to_draw, image);
        }
    }

    // Dibujo encima las lineas que definen la cuadricula
    dibujar_cuadricula(painter, this->tile_size, width, height);
}
