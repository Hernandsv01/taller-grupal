#include "editormapa.h"


#include<QDebug>

EditorMapa::EditorMapa(QWidget* parent)
    : RenderizadoMapa(parent)
{
    this->setMouseTracking(true);
}


void EditorMapa::add_tile_selection(QListView* list_tile_selection) {
    this->tile_selection = list_tile_selection;
}

void EditorMapa::mousePressEvent(QMouseEvent* event) {

    this->isEditing = true;

    if (event->button() == Qt::LeftButton) {
        auto index_selected_tile = this->tile_selection->currentIndex();
        if (!index_selected_tile.isValid())
            return;

        tile_a_pintar = this->tiles->itemFromIndex(index_selected_tile)->data(Qt::UserRole +3).value<Tile>();
    } else {
        tile_a_pintar = Tile::air;
    }

    this->mouseMoveEvent(event);
}

void EditorMapa::mouseMoveEvent(QMouseEvent* event) {
    if (!this->isEditing)
        return;


    qDebug() << "EVENTO" << event->x() << " " << event->y();
    int x_grilla = event->x() / tile_size;
    int y_grilla = event->y() / tile_size;


    qDebug() << "GRILLA" << x_grilla << " " << y_grilla;

    if (x_grilla >= x_limit || x_grilla < 0) {
        qDebug("Se intentó dibujar en x = %d. "
               "Pero solo existe nivel hasta x = %d",
               x_grilla, x_limit);
        return;
    }

    if (y_grilla >= y_limit || y_grilla < 0) {
        qDebug("Se intentó dibujar en y = %d. "
               "Pero solo existe nivel hasta y = %d",
               y_grilla, y_limit);
        return;
    }

    level[x_grilla][y_grilla] = tile_a_pintar;
    this->repaint();

}
void EditorMapa::mouseReleaseEvent(QMouseEvent* event) {
    isEditing = false;
}

void EditorMapa::wheelEvent(QWheelEvent* event) {return;}


