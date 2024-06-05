#ifndef GUILOOP_H
#define GUILOOP_H

#define FPS 60
#define SECOND_IN_NANO 1000000000

#define TICK_DURATION (std::chrono::nanoseconds{(SECOND_IN_NANO / FPS)})

#define RENDER_DURATION (std::chrono::nanoseconds{((SECOND_IN_NANO / FPS) / 2)})

#include <chrono>

#include "../../common/library/thread.h"
#include "estado_juego.h"
#include "render.h"

class GuiLoop : public Thread {
    std::chrono::steady_clock reloj;

    Window& window_for_render;
    Render* render = nullptr;

    // TODO: estado que Mariano debe actualizar,
    // TODO: a partir de las updates que recibe de un Queue?
    EstadoJuegoActualizable estadoJuegoActualizable;

    uint32_t tick_actual;

   public:
    // GuiLoop();
    explicit GuiLoop(Window& window);
    ~GuiLoop();

   private:
    void run() override;

    void inicializar_render();

    // void stop_custom() override;

    // std::string text_description() override;

    void ejecutar_renderer();

    void ejecutar_renderer(MapInfo& mapInfo);

    void actualizar_estado();
};

#endif
