#include "editormapa.h"

#include <QDebug>
#include <QMetaType>
#include <iostream>

// cppcheck-suppress unknownMacro
Q_DECLARE_METATYPE(Block)

MapEditor::MapEditor(Map& map, QWidget* parent) : MapRenderer(map, parent) {
    this->setMouseTracking(true);

    // TODO: ELIMINAR
    // //  Defino algunos tiles en la grilla para testear.
    // map.add_block(Coordinate{0, 0}, Block{Collision::Cube, "dirt"});

    // map.add_block(Coordinate{5, 3}, Block{Collision::Cube, "water"});

    // map.add_block(Coordinate{4, 2}, Block{Collision::Cube, "stone"});
}

void MapEditor::add_tile_selection(QListView* list_tile_selection) {
    // Guardo el puntero a Widget de la lista de seleccion de tiles.
    this->tile_selection = list_tile_selection;
}

void MapEditor::mousePressEvent(QMouseEvent* event) {
    // Ejecuta a al funcion padre por si quiere hacer algo con el evento;
    this->MapRenderer::mousePressEvent(event);

    if (event->button() == Qt::MouseButton::LeftButton) {
        this->isEditing = true;
        // Si se clickeo con el boton izquierdo, busco cual es el tile
        // seleccionado en el Widget
        auto index_selected_tile = this->tile_selection->currentIndex();
        if (!index_selected_tile.isValid()) return;

        tile_to_paint = this->tiles->itemFromIndex(index_selected_tile)
                            ->data(Qt::UserRole + 3)
                            .value<Block>();
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
    this->MapRenderer::mouseMoveEvent(event);

    // Si no estoy editando, no quiero modificar nada.
    if (!this->isEditing) return;

    // Calculo en que posicion de la grilla estoy parado.
    coord_unit x_grid = (event->x() - camera_reference.x()) / tile_size;
    coord_unit y_grid = (event->y() - camera_reference.y()) / tile_size;

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
    map.add_block(Coordinate{x_grid, y_grid}, tile_to_paint);

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
    qDebug() << "Guardando mapa";
    map.toYaml();
}