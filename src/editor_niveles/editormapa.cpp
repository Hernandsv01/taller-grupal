#include "editormapa.h"

#include <QDebug>
#include <QMetaType>
#include <iostream>

// cppcheck-suppress unknownMacro
Q_DECLARE_METATYPE(Block)

MapEditor::MapEditor(QWidget* parent) : MapRenderer(parent) {
    this->setMouseTracking(true);

    // TODO: ELIMINAR
    // //  Defino algunos tiles en la grilla para testear.
    // map.add_block(Coordinate{0, 0}, Block{Collision::Cube, "dirt"});

    // map.add_block(Coordinate{5, 3}, Block{Collision::Cube, "water"});

    // map.add_block(Coordinate{4, 2}, Block{Collision::Cube, "stone"});
}

void MapEditor::mousePressEvent(QMouseEvent* event) {
    // Ejecuta a al funcion padre por si quiere hacer algo con el evento;
    this->MapRenderer::mousePressEvent(event);

    if (event->button() == Qt::MouseButton::LeftButton) {
        this->isEditing = true;
        // Si se clickeo con el boton izquierdo, obtengo el tile seleccionado en
        // la lista
        tile_to_paint = current_selected_tile;

    } else if (event->button() == Qt::MouseButton::RightButton) {
        // Si se clickeo con el boton derecho, se pinta aire (osea, se
        // borra).
        this->isEditing = true;
        tile_to_paint = Block{Collision::Air, ""};
    } else {
        return;
    }

    // Llamo al evento de mouseMove, que es el que realmente realiza la accion
    // de modificar la grilla.
    this->mouseMoveEvent(event);
}

void MapEditor::mouseMoveEvent(QMouseEvent* event) {
    MapRenderer::checkMapAvaible();

    this->MapRenderer::mouseMoveEvent(event);

    // Si no estoy editando, no quiero modificar nada.
    if (!this->isEditing) return;

    // Calculo en que posicion de la grilla estoy parado.
    coord_unit x_grid = (event->x() - camera_reference.x()) / tile_size;
    coord_unit y_grid = (event->y() - camera_reference.y()) / tile_size;

    auto limits = (*map)->get_map_size();
    auto x_limit = limits.x;
    auto y_limit = limits.y;

    // Verifico que no me pase de los limites de la grilla. (puede pasar si
    // moves mouse muy rapido fuera de ventana)
    if (x_grid >= x_limit || x_grid < 0) {
        qDebug(
            "Se intentó dibujar en x = %d. "
            "Pero solo existe nivel hasta x = %d",
            x_grid, x_limit);
        return;
    }

    if (y_grid >= y_limit || y_grid < 0) {
        qDebug(
            "Se intentó dibujar en y = %d. "
            "Pero solo existe nivel hasta y = %d",
            y_grid, y_limit);
        return;
    }

    // Modifico la representacion en el mapa.
    (*map)->add_block(Coordinate{x_grid, y_grid}, tile_to_paint);

    // Repinto el widget generando un evento de pintado.
    // (Esto llama al metodo MapRenderer::paintEvent())
    this->update();
}
void MapEditor::mouseReleaseEvent(QMouseEvent* event) {
    this->MapRenderer::mouseReleaseEvent(event);
    // Si suelto el mouse, dejo de editar

    if (event->button() == Qt::MouseButton::LeftButton ||
        event->button() == Qt::MouseButton::RightButton)
        this->isEditing = false;
}

void MapEditor::wheelEvent(QWheelEvent* event) {
    this->MapRenderer::wheelEvent(event);
}

MapEditor::~MapEditor() {}

void MapEditor::saveMap() {
    checkMapAvaible();
    qDebug() << "Guardando mapa";
    (*map)->toYaml();
}

void MapEditor::changeSelectedTile(Block newSelectedTile) {
    // Se ejecuta en el mismo hilo en el que se accede,
    // por lo que no hay race condition.
    this->current_selected_tile = newSelectedTile;
}

void MapEditor::changeBackground(IdTexture background) {
    // Se ejecuta en el mismo hilo en el que se accede,
    // por lo que no hay race condition.
    (*map)->set_background(background);
}