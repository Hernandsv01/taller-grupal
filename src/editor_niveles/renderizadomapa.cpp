#include "renderizadomapa.h"

#include <QDebug>
#include <QVector>
#include <algorithm>
#include <cmath>
#include <iostream>

#define MAX_TILE_SIZE 120.0
#define MIN_TILE_SIZE 10.0

void MapRenderer::checkMapAvaible() {
    if (!map.has_value()) {
        throw std::runtime_error("Mapa no definido");
    }
}

void MapRenderer::setMap(Map* map) {
    this->map = map;
    this->x_limit = map->get_map_size().x;
    this->y_limit = map->get_map_size().y;
}

void MapRenderer::drawBackground(QPainter& painter) {
    if (!background.has_value()) return;

    painter.setBackgroundMode(Qt::BGMode::OpaqueMode);
    painter.setBackground(QBrush(*background));

    painter.setBrush(QBrush(*background));
    QRect rectangle(camera_reference, QSize(this->x_limit * this->tile_size,
                                            this->y_limit * this->tile_size));

    painter.drawRect(rectangle);
}

void MapRenderer::setBackground(QImage image) { background = image; }

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

MapRenderer::MapRenderer(QWidget* parent)
    : QWidget{parent}, tiles{nullptr}, map(std::nullopt) {
    // Defino el tilesize. Esto indica cuantos pixeles ocupa cada casillero de
    // la grilla.
    this->tile_size = 64;
}

MapRenderer::~MapRenderer() {
    if (tiles) delete tiles;
}

void MapRenderer::addTileModel(QStandardItemModel* newTiles) {
    this->tiles = newTiles;
}

void MapRenderer::paintEvent(QPaintEvent* event) {
    checkMapAvaible();

    QPainter painter(this);

    int width = this->width();
    int height = this->height();

    drawBackground(painter);

    std::vector<BlockOnlyTexture> blockTextures =
        (*map)->get_all_block_textures_editor();

    for (const BlockOnlyTexture& block : blockTextures) {
        // Busco el item en la lista de tiles
        // (por ahora necesito el nombre. Tal vez hay alguna manera
        // mejor ?)

        int x = (int)block.coordinate.x * tile_size + camera_reference.x();
        int y = (int)block.coordinate.y * tile_size + camera_reference.y();

        // Esta fuera de la pantalla
        if (x + tile_size < 0 || x > width || y + tile_size < 0 || y > height)
            continue;

        QList<QStandardItem*> tile_items =
            tiles->findItems(QString::fromStdString(block.texture));

        if (tile_items.isEmpty()) continue;  // Probablemente sea aire

        QStandardItem* tile_item = tile_items[0];

        // Obtengo la imagen del tile
        QVariant image_variant = tile_item->data(Qt::UserRole + 2);

        QImage image = image_variant.value<QImage>();

        // Dibujo el tile en la posicion correcta.
        QRect rectangle_to_draw(x, y, tile_size, tile_size);

        painter.drawImage(rectangle_to_draw, image);
    }

    // // Loop para recorrer la grilla de tiles y dibujarlos
    // for (uint y = 0; y <= y_limit; y++) {
    //     // En el caso de que quiera renderizar algo
    //     // que este fuera de lo definido por level
    //     if (y >= y_limit) break;

    //     for (uint x = 0; x <= x_limit; x++) {
    //         // En el caso de que quiera renderizar algo
    //         // que este fuera de lo definido por level
    //         if (x >= x_limit) break;

    //         // Obtengo el tipo de tile, si es aire lo omito.
    //         Block block = level[x][y];
    //         if (block.texture == "") continue;

    //         // Busco el item en la lista de tiles
    //         // (por ahora necesito el nombre. Tal vez hay alguna manera
    //         mejor?) QList<QStandardItem*> tile_items =
    //             tiles->findItems(QString::fromStdString(block.texture));

    //         if (tile_items.isEmpty()) continue;  // Probablemente sea aire

    //         QStandardItem* tile_item = tile_items[0];

    //         // Obtengo la imagen del tile
    //         QVariant image_variant = tile_item->data(Qt::UserRole + 2);

    //         QImage image = image_variant.value<QImage>();

    //         // Dibujo el tile en la posicion correcta.
    //         QRect rectangle_to_draw(x * tile_size + camera_reference.x(),
    //                                 y * tile_size + camera_reference.y(),
    //                                 tile_size, tile_size);

    //         painter.drawImage(rectangle_to_draw, image);
    //     }
    // }

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
        this->update();
    }
}

void MapRenderer::wheelEvent(QWheelEvent* event) {
    int numDegrees = event->angleDelta().y() / 8;
    int numSteps = numDegrees / 15;

    float modify = 3;

    if (tile_size + modify * numSteps > MAX_TILE_SIZE ||
        tile_size + modify * numSteps < MIN_TILE_SIZE)
        return;

    QPointF window_center = QPointF(this->width(), this->height()) / 2.0;

    QPointF window_center_on_grid =
        (window_center - QPointF(camera_reference)) / tile_size;

    tile_size = round(tile_size + modify * numSteps);

    // Despejo de la ecuacion de arriba la camara reference para el nuevo
    // tile_size.
    QPointF new_camera_reference =
        window_center - (tile_size * window_center_on_grid);

    set_camera_reference(new_camera_reference.toPoint());

    this->update();
}
