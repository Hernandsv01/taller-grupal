#ifndef EDITORMAPA_H
#define EDITORMAPA_H

#include <renderizadomapa.h>

#include <QListView>
#include <mutex>

class MapEditor : public MapRenderer {
    bool isEditing = false;
    bool isMovingCamera = false;
    Block tile_to_paint = Block{Collision::Air, ""};

    // Necesito mutex, porque el current_selected_tile es accedido
    // por el hilo de la mainwindow, y el hilo de la interfaz.
    // Tal vez son el mismo hilo, pero prefiero no arriesgarme
    std::mutex mutex_current_selected_tile;
    Block current_selected_tile = Block{Collision::Air, ""};

   public:
    MapEditor(QWidget* parent);
    virtual ~MapEditor();

    void saveMap();

    void changeSelectedTile(Block newSelectedTile);

   private slots:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void wheelEvent(QWheelEvent* event);
};

#endif  // EDITORMAPA_H
