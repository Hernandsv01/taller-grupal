#include "gui_loop.h"

#include <iostream>
#include <thread>

#include "../update_queue.h"

GuiLoop::GuiLoop(Window& window)
    : Thread("GuiLoop cliente"), currentTick(0), windowForRender(window) {
    // Harcodeo un player dummy. En la version final del juego, esto lo
    // recibiría del servidor.
    PlayerState player;
    player.direction = Direction::Right;
    player.id = 0;
    player.position = Position{100, 200};
    player.healthPoints = 10;
    player.characterType = CharacterType::Jazz;
    player.score = 0;

    updatableGameState.addMainPlayer(player);
};

void GuiLoop::stop_custom() {
    // No tengo que hacer nada, ya que ninguna funcion de GuiLoop, es
    // bloqueando, por lo que el thread va a terminar cuando termine la
    // iteracion actual, y keep_running() sea false.
}

void GuiLoop::initializeRender() { render = new Render(windowForRender); }

GuiLoop::~GuiLoop() {
    delete render;
    render = nullptr;
}

void GuiLoop::run() {
    // Estoy obligado a construir el renderer acá, porque para que el renderer
    // pueda dibujar en pantalla, necesita ejecutarse en el mismo thread donde
    // fue construido.
    initializeRender();

    using namespace std::chrono;
    time_point tickInitialTime = clock.now();
    while (this->keep_running()) {
        /*
    tickInitialTime     tickEndTime
            ↓               ↓
            |-------*-------|
                    ↑
                currentTime
        */

        time_point tickEndTime = tickInitialTime + TICK_DURATION;

        updateGameState();

        time_point currentTime = clock.now();
        bool isTimeLeftInTick = currentTime <= tickEndTime;

        if (isTimeLeftInTick) {
            // Ejecuto el renderer, solamente si todavía me sobra tiempo en el
            // tick actual.
            // En el caso de que ya haya consumido todo el tiempo del tick
            // actual, Decido ni siquiera ejecutar el renderer para , tal vez,
            // llegar al proximo tick a tiempo.
            runRenderer();
        } else {
#ifndef NDEBUG
            std::cout << "Render cancelado. ";
#endif
        }

        currentTime = clock.now();
        isTimeLeftInTick = currentTime <= tickEndTime;

        // Tengo otro if, porque podría haber sobrado tiempo para renderizar,
        // pero haberme pasado del tiempo mientras renderizaba.
        // Por lo que tengo que checkear otra vez
        if (!isTimeLeftInTick) {
            // Calculo la cantidad de ticks completos que usé
            uint excessTicksElapsed =
                (currentTime - tickInitialTime) / TICK_DURATION;

            // Agrego cantidad de ticks dropeados
            currentTick += excessTicksElapsed;

#ifndef NDEBUG
            std::cout << excessTicksElapsed << " ticks dropeados\n";
#endif

            // Agregado cantidad de tiempo excedido ("dropeado")
            tickInitialTime += (TICK_DURATION * excessTicksElapsed);
            tickEndTime = tickInitialTime + TICK_DURATION;
        }

        tickInitialTime += TICK_DURATION;
        currentTick += 1;
        std::this_thread::sleep_until(tickEndTime);
    }
}

void GuiLoop::runRenderer() {
    // Genero un nuevo estado apto para que lo consuma el render.
    GameStateRenderer gameStateRenderer = updatableGameState.getStateRenderer();

    if (render == nullptr)
        throw std::runtime_error(
            "Se debe inicializar el render antes de usarlo");

    render->presentPlayer(gameStateRenderer.mainPlayer);
}

void GuiLoop::updateGameState() {
    // Obtener todas las updates encoladas
    std::vector<Update> all_updates = Update_queue::try_pop_all();

    // aplicar de a una las updates en orden (las ultimas son las más
    // recientes).
    // Si no hay ninguna update, no se updatea nada.
    for (Update update : all_updates) {
        updatableGameState.handleUpdate(update);
    }
}