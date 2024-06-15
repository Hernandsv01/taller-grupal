#ifndef EDITORMAPA_H
#define EDITORMAPA_H

#include <renderizadomapa.h>

#include <QListView>

class MapEditor : public MapRenderer {
    bool isEditing = false;
    bool isMovingCamera = false;
    QListView* tile_selection = nullptr;
    Tile tile_to_paint = Tile::air;

   public:
    MapEditor(QWidget* parent);

    void add_tile_selection(QListView* list_tile_selection);

   private slots:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void wheelEvent(QWheelEvent* event);
};

#endif  // EDITORMAPA_H
