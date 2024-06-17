#ifndef EDITORMAPA_H
#define EDITORMAPA_H

#include <renderizadomapa.h>

#include <QListView>
#include <mutex>

class MapEditor : public MapRenderer {
    bool isEditing = false;
    bool isMovingCamera = false;
    Block tile_to_paint = Block{Collision::Air, ""};

    // Esta variable es accedida tanto por mainwindow como por el editor.
    // No le pongo mutex porque verifiqué que ambas partes del programa
    // se ejecutan en el mismo hilo.
    Block current_selected_tile = Block{Collision::Air, ""};

   public:
    MapEditor(QWidget* parent);
    virtual ~MapEditor();

    void saveMap();

    void changeSelectedTile(Block newSelectedTile);
    void changeBackground(IdTexture image);

   private slots:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void wheelEvent(QWheelEvent* event);
};

#endif  // EDITORMAPA_H
