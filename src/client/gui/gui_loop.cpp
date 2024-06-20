#include "gui_loop.h"

#include <iostream>
#include <thread>

#include "../update_queue.h"

GuiLoop::GuiLoop(Window& window, uint16_t player_id, std::string map_name)
    : Thread("GuiLoop cliente"),
      currentTick(0),
      windowForRender(window),
      mapName(map_name) {
    // Harcodeo un player dummy. En la version final del juego, esto lo
    // recibiría del servidor.
    PlayerState player;
    player.direction = enums_value_update::Direction::Right;
    player.id = player_id;
    player.position = Position{0, 0};
    player.healthPoints = 10;
    player.characterType = CharacterType::Jazz;
    player.score = 0;
    player.state = State(enums_value_update::Player_State_Enum::Idle, 0);

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
    ////////   Harcodeo un mapa   ////////
    int groundPosY = 0;
    std::vector<Position> positionGround;
    for (int i = 0; i < 640; i += 32) {
        positionGround.push_back(Position{i, groundPosY});
    }
    std::vector<Position> positionUnder;
    for (int i = 0; i < 640; i += 32) {
        positionUnder.push_back(Position{i, 64});
    }
    MapInfo mapInfo;
    mapInfo.mapTexture = Diamond;
    mapInfo.typeOfGround = RightSign;
    mapInfo.groundPosition = positionGround;
    mapInfo.typeOfUnder = RedDiamond;
    mapInfo.underPosition = positionUnder;
    //////////////

    // Estoy obligado a construir el renderer acá, porque para que el renderer
    // pueda dibujar en pantalla, necesita ejecutarse en el mismo thread donde
    // fue construido.
    initializeRender();

    using namespace std::chrono;
    time_point tickInitialTime = clock.now();
    while (this->keep_running() && !matchEnded) {
        /*
    tickInitialTime     tickEndTime
            ↓               ↓
            |-------*-------|
                    ↑
                currentTime
        */

        time_point tickEndTime = tickInitialTime + TICK_DURATION;

        updateGameState();
        matchEnded = updatableGameState.hasMatchEnded();

        time_point currentTime = clock.now();
        bool isTimeLeftInTick = currentTime <= tickEndTime;

        if (isTimeLeftInTick) {
            // Ejecuto el renderer, solamente si todavía me sobra tiempo en el
            // tick actual.
            // En el caso de que ya haya consumido todo el tiempo del tick
            // actual, Decido ni siquiera ejecutar el renderer para , tal vez,
            // llegar al proximo tick a tiempo.

            runRenderer(mapInfo);
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

void GuiLoop::updateGameState() {
    // Obtener todas las updates encoladas
    std::vector<Update::Update_new> all_updates = Update_queue::try_pop_all();

    // aplicar de a una las updates en orden (las ultimas son las más
    // recientes).
    // Si no hay ninguna update, no se updatea nada.
    for (Update::Update_new update : all_updates) {
        updatableGameState.handleUpdate(update, currentTick);
    }
}

void GuiLoop::runRenderer(MapInfo& mapInfo) {
    // Genero un nuevo estado apto para que lo consuma el renderer
    GameStateRenderer gameStateRenderer =
        updatableGameState.getStateRenderer(currentTick);

    if (render == nullptr)
        throw std::runtime_error(
            "Se debe inicializar el render antes de usarlo");

    render->presentGame(gameStateRenderer, mapInfo);

    // std::cout << "tick: " << tick_actual << "\n";
    // std::cout << "(" << gameStateRenderer.mainPlayer.position.x
    // << ", "
    //           << gameStateRenderer.mainPlayer.position.y << ")"
    //           << std::endl;
}

bool GuiLoop::hasMatchEnded() { return matchEnded; }