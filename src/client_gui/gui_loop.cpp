#include "gui_loop.h"

#include <iostream>
#include <thread>

double get_random() {
    static int x = 0;
    double random[] = {1.1, 1.2, 1.7, 5, 2.6};
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

        // Procesado de cosas
        {
            auto duracion_render = (std::chrono::nanoseconds{
                (int)((SECOND_IN_NANO / FPS) * get_random())});

            std::this_thread::sleep_for(duracion_render);
            std::cout << "TICK " << tick_actual << std::endl;
            std::cout << "DURACION:" << std::endl;
            std::cout << "\tRender: "
                      << duration_cast<milliseconds>(duracion_render).count()
                      << "ms" << std::endl;
        }

        time_point final_tick = inicio_tick + TICK_DURATION;
        time_point tiempo_actual = reloj.now();

        if (tiempo_actual < inicio_tick) {
            std::cout << "ERROR" << std::endl;
        }

        bool sobra_tiempo_en_tick = tiempo_actual <= final_tick;

        if (!sobra_tiempo_en_tick) {
            // Calculo la cantidad de ticks completos que usé
            uint ticks_extras_transcurridos =
                (tiempo_actual - inicio_tick) / TICK_DURATION;

            // Agrego cantidad de ticks dropeados
            tick_actual += ticks_extras_transcurridos;

            // Agregado cantidad de tiempo salteado ("dropeado")
            inicio_tick += (TICK_DURATION * ticks_extras_transcurridos);
            final_tick = inicio_tick + TICK_DURATION;
        }

        inicio_tick += TICK_DURATION;
        tick_actual += 1;
        std::this_thread::sleep_until(final_tick);
    }
}