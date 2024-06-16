#ifndef EDITORMAPA_H
#define EDITORMAPA_H

#include <renderizadomapa.h>

#include <QListView>

class MapEditor : public MapRenderer {
    bool isEditing = false;
    bool isMovingCamera = false;
    QListView* tile_selection = nullptr;
    Block tile_to_paint = Block{Collision::Air, ""};

   public:
    MapEditor(Map& map, QWidget* parent);
    virtual ~MapEditor();

    void add_tile_selection(QListView* list_tile_selection);

   private slots:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void wheelEvent(QWheelEvent* event);
};

#endif  // EDITORMAPA_H
