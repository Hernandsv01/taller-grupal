#include "renderizadomapa.h"

#include <QDebug>
#include <QVector>
#include <algorithm>
#include <cmath>
#include <iostream>

#define MAX_TILE_SIZE 120.0
#define MIN_TILE_SIZE 10.0

void MapRenderer::drawGrid(QPainter& painter) {
    QVector<QLine> lineas;

    int first_x = 0 + camera_reference.x();
    int last_x = (this->x_limit * this->tile_size) + camera_reference.x();

    int first_y = 0 + camera_reference.y();
    int last_y = (this->y_limit * this->tile_size) + camera_reference.y();

    // Agrego a un vector todas las lineas horizontales y verticales a dibujar
    for (int x = first_x; x <= last_x; x += this->tile_size) {
        QLine linea(x, first_y, x, last_y);
        lineas.push_back(linea);
    }

    for (int y = first_y; y <= last_y; y += this->tile_size) {
        QLine linea(first_x, y, last_x, y);
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
    for (uint y = 0; y <= y_limit; y++) {
        // En el caso de que quiera renderizar algo
        // que este fuera de lo definido por level
        if (y >= y_limit) break;

        for (uint x = 0; x <= x_limit; x++) {
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
            QRect rectangle_to_draw(x * tile_size + camera_reference.x(),
                                    y * tile_size + camera_reference.y(),
                                    tile_size, tile_size);

            painter.drawImage(rectangle_to_draw, image);
        }
    }

    // Dibujo encima las lineas que definen la cuadricula
    drawGrid(painter);
}

void MapRenderer::set_camera_reference(QPoint camera_reference) {
    this->camera_reference = camera_reference;
}

void MapRenderer::modify_camera_reference(QPoint delta) {
    this->camera_reference += delta;
}

void MapRenderer::mousePressEvent(QMouseEvent* event) {
    // Defino que se está editando la grilla.
    if (event->button() == Qt::MouseButton::MiddleButton) {
        this->moving_camera = true;
        mouse_clicked_reference = event->pos();
    }
}

void MapRenderer::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::MiddleButton) {
        this->moving_camera = false;
    }
}

void MapRenderer::mouseMoveEvent(QMouseEvent* event) {
    if (this->moving_camera) {
        QPoint current_mouse_position = event->pos();
        QPoint delta = current_mouse_position - mouse_clicked_reference;

        this->modify_camera_reference(delta);

        mouse_clicked_reference = current_mouse_position;
        this->repaint();
    }
}

void MapRenderer::wheelEvent(QWheelEvent* event) {
    int numDegrees = event->angleDelta().y() / 8;
    int numSteps = numDegrees / 15;

    float modify = 3;

    if (tile_size + modify * numSteps > MAX_TILE_SIZE ||
        tile_size + modify * numSteps < MIN_TILE_SIZE)
        return;

    tile_size = round(tile_size + modify * numSteps);

    qDebug() << "Tile size: " << tile_size << " modify: " << modify;

    this->repaint();
}
