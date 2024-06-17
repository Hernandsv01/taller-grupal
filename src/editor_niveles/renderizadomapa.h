#ifndef RENDERIZADOMAPA_H
#define RENDERIZADOMAPA_H

#include <QPaintEvent>
#include <QPainter>
#include <QWidget>
#include <optional>

#include "../common/map/map.h"

class MapRenderer : public QWidget {
    Q_OBJECT

   protected:
    std::optional<Map*> map;

    // Usado para el zoom
    uint tile_size = 64;
    QPoint camera_reference = QPoint(0, 0);

   private:
    QPoint mouse_clicked_reference = QPoint(0, 0);
    bool moving_camera = false;
    QMap<IdTexture, QImage> tile_textures;
    QMap<IdTexture, QImage> background_textures;

   public:
    explicit MapRenderer(QWidget* parent = nullptr);
    virtual ~MapRenderer();

    // Setea un mapa para que el renrerer use.
    void setMap(Map* map);

    // Agrega diccionaros para las texturas. De manera que a partir del
    // IdTexture, pueda acceder a la imagen para renderizar.
    void addTileTextures(QMap<IdTexture, QImage> tile_textures);
    void addBackgroundTextures(QMap<IdTexture, QImage> background_textures);

   private:
    // Se llama cada vez que se actualiza la pantalla.
    void paintEvent(QPaintEvent* event);

    void set_camera_reference(QPoint reference);
    void modify_camera_reference(QPoint delta);

   protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void checkMapAvaible();

   private:
    void drawGrid(QPainter& painter);
    void drawBackground(QPainter& painter);

   signals:
};

#endif  // RENDERIZADOMAPA_H
