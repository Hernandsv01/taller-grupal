#include "editormapa.h"

#include <QDebug>

MapEditor::MapEditor(QWidget* parent) : MapRenderer(parent) {
    this->setMouseTracking(true);
}

void MapEditor::add_tile_selection(QListView* list_tile_selection) {
    // Guardo el puntero a Widget de la lista de seleccion de tiles.
    this->tile_selection = list_tile_selection;
}

void MapEditor::mousePressEvent(QMouseEvent* event) {
    // Defino que se está editando la grilla.
    this->isEditing = true;

    if (event->button() == Qt::LeftButton) {
        // Si se clickeo con el boton izquierdo, busco cual es el tile
        // seleccionado en el Widget
        auto index_selected_tile = this->tile_selection->currentIndex();
        if (!index_selected_tile.isValid()) return;

        tile_to_paint = this->tiles->itemFromIndex(index_selected_tile)
                            ->data(Qt::UserRole + 3)
                            .value<Tile>();
    } else {
        // Si se clickeo con el boton derecho, se pinta aire (osea, se borra).
        tile_to_paint = Tile::air;
    }

    // Llamo al evento de mouseMove, que es el que realmente realiza la accion
    // de modificar la grilla.
    this->mouseMoveEvent(event);
}

void MapEditor::mouseMoveEvent(QMouseEvent* event) {
    // Si no estoy editando, no quiero modificar nada.
    if (!this->isEditing) return;

    qDebug() << "EVENTO" << event->x() << " " << event->y();

    // Calculo en que posicion de la grilla estoy parado.
    int x_grid = event->x() / tile_size;
    int y_grid = event->y() / tile_size;

    qDebug() << "GRILLA" << x_grid << " " << y_grid;

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

    // Modifico la representacion de la grilla.
    level[x_grid][y_grid] = tile_to_paint;

    // Repinto el widget generando un evento de pintado.
    // (Esto llama al metodo MapRenderer::paintEvent())
    this->repaint();
}
void MapEditor::mouseReleaseEvent(QMouseEvent* event) {
    // Si suelto el mouse, dejo de editar
    isEditing = false;
}

void MapEditor::wheelEvent(QWheelEvent* event) {
    // Tal vez implemente zoom en el futuro?
    return;
}
