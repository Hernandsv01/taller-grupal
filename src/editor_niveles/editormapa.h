#ifndef EDITORMAPA_H
#define EDITORMAPA_H

#include <renderizadomapa.h>
#include <QListView>


class EditorMapa: public RenderizadoMapa
{
    bool isEditing = false;
    QListView* tile_selection = nullptr;
    Tile tile_a_pintar = Tile::air;


public:
    EditorMapa(QWidget* parent);

    void add_tile_selection(QListView* list_tile_selection);

private slots:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void wheelEvent(QWheelEvent* event);
};

#endif // EDITORMAPA_H
