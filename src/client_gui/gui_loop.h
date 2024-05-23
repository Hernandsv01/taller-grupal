#ifndef GUILOOP_H
#define GUILOOP_H

#define FPS 60
#define SECOND_IN_NANO 1000000000

#define TICK_DURATION (std::chrono::nanoseconds{(SECOND_IN_NANO / FPS)})

#define RENDER_DURATION (std::chrono::nanoseconds{((SECOND_IN_NANO / FPS) / 2)})

#include <chrono>

class GuiLoop {
    std::chrono::steady_clock reloj;

    // TODO: Acá matias debería poner el renderer para que se cree
    // Renderer renderer;

    // TODO: estado que Mariano debe actualizar,
    // TODO: a partir de las updates que recibe de un Queue?
    // EstadoJuego estadoActual;
    // Queue<update> queueUpdates;

    uint32_t tick_actual;

   public:
    GuiLoop();

    void ejecutar();

   private:
    void ejecutar_renderer();

    void actualizar_estado();
};

#endif
