#include "gui_loop.h"

#include <iostream>
#include <thread>

double get_random() {
    static int x = 0;
    double random[] = {0.90, 1.0, 1.7, 5, 2.6};
    x = (x + 1) % (sizeof(random) / sizeof(double));

    return random[x];
}

GuiLoop::GuiLoop() : tick_actual(0){};

void GuiLoop::ejecutar() {
    using namespace std::chrono;
    time_point INICIO_ABSOLUTO = reloj.now();
    time_point inicio_tick = INICIO_ABSOLUTO;

    while (true) {
        /*

        inicio_tick     final_tick
            ↓               ↓
            |-------*-------|
                    ↑
                tiempo_actual

        */

        // // Procesado de cosas
        // {
        //     auto duracion_render = (std::chrono::nanoseconds{
        //         (int)((SECOND_IN_NANO / FPS) * get_random())});

        //     std::this_thread::sleep_for(duracion_render);
        // }

        time_point final_tick = inicio_tick + TICK_DURATION;

        actualizar_estado();

        time_point tiempo_actual = reloj.now();
        bool sobra_tiempo_en_tick = tiempo_actual <= final_tick;

        if (sobra_tiempo_en_tick) {
            // Ejecuto el renderer, solamente si todavía me sobra tiempo en el
            // tick actual.
            // En el caso de que ya haya consumido todo el tiempo del tick
            // actual, Decido ni siquiera ejecutar el renderer para , tal vez,
            // llegar al proximo tick a tiempo.
            ejecutar_renderer();
        } else {
            std::cout << "Render cancelled. ";
        }

        tiempo_actual = reloj.now();
        sobra_tiempo_en_tick = tiempo_actual <= final_tick;

        // Tengo otro if, porque podría haber sobrado tiempo para renderizar,
        // pero haberme pasado del tiempo mientras renderizaba.
        // Por lo que tengo que checkear otra vez
        if (!sobra_tiempo_en_tick) {
            // Calculo la cantidad de ticks completos que usé
            uint ticks_extras_transcurridos =
                (tiempo_actual - inicio_tick) / TICK_DURATION;

            // Agrego cantidad de ticks dropeados
            tick_actual += ticks_extras_transcurridos;

            std::cout << ticks_extras_transcurridos << " dropped ticks\n";

            // Agregado cantidad de tiempo salteado ("dropeado")
            inicio_tick += (TICK_DURATION * ticks_extras_transcurridos);
            final_tick = inicio_tick + TICK_DURATION;
        }

        inicio_tick += TICK_DURATION;
        tick_actual += 1;
        std::this_thread::sleep_until(final_tick);
    }
}

void GuiLoop::ejecutar_renderer() {
    // TODO: acá matias debería poner el llamado al renderr
    // renderer.renderizar(tick_actual, estadoActual);

    std::cout << "tick: " << tick_actual << "\n";
    return;
}

void GuiLoop::actualizar_estado() {
    // TODO: Acá deberia codear la manera de actualizar el estado actual
    // while (true) {
    //     auto update = queueUpdates.tryrecv();
    //     if (update == None)
    //         break:

    //     estadoActual.actualizar(update);
    // }
}