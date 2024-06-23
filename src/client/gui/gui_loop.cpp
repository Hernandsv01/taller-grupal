#include "gui_loop.h"

#include <iostream>
#include <thread>

#include "../update_queue.h"
#include "textureManager.h"

GuiLoop::GuiLoop(Window& window, uint16_t player_id, std::string map_name)
    : Thread("GuiLoop cliente"),
      clock(),
      windowForRender(window),
      render(nullptr),
      gameState(),  
    // updatableGameState(?),
      mainId(player_id),
      currentTick(0),
      map(Map::fromYaml(map_name)){
          // Harcodeo un player dummy. En la version final del juego, esto lo
          // recibiría del servidor.
      };

void GuiLoop::stop_custom() {
    // No tengo que hacer nada, ya que ninguna funcion de GuiLoop, es
    // bloqueando, por lo que el thread va a terminar cuando termine la
    // iteracion actual, y keep_running() sea false.
}

void GuiLoop::initializeRender() {
    render = new Render(windowForRender, mainId);
}

GuiLoop::~GuiLoop() {
    delete render;
    render = nullptr;
}

void GuiLoop::run() {
    ////////   Harcodeo un mapa   ////////
    // int groundPosY = 0;
    // std::vector<Position> positionGround;
    // for (int i = 0; i < 640; i += 32) {
    //     positionGround.push_back(Position{i, groundPosY});
    // }
    // std::vector<Position> positionUnder;
    // for (int i = 0; i < 640; i += 32) {
    //     positionUnder.push_back(Position{i, 64});
    // }
    // MapInfo mapInfo;
    // mapInfo.mapTexture = Diamond;
    // mapInfo.typeOfGround = RightSign;
    // mapInfo.groundPosition = positionGround;
    // mapInfo.typeOfUnder = RedDiamond;
    // mapInfo.underPosition = positionUnder;
    //////////////

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

void GuiLoop::updateGameState() {
    // Obtener todas las updates encoladas
    std::vector<Update::Update_new> all_updates = Update_queue::try_pop_all();

    // aplicar de a una las updates en orden (las ultimas son las más
    // recientes).
    // Si no hay ninguna update, no se updatea nada.
    static bool paso = false;

    if (!paso) {
        std::vector<std::pair<Update::EntityType, Update::EntitySubtype>>
            entities = {
                {Update::EntityType::Player, Update::EntitySubtype::Jazz},
                {Update::EntityType::Enemy, Update::EntitySubtype::Enemy1},
                {Update::EntityType::Enemy, Update::EntitySubtype::Enemy2},
                {Update::EntityType::Enemy, Update::EntitySubtype::Enemy3},
                {Update::EntityType::Bullet, Update::EntitySubtype::No_subtype},
                {Update::EntityType::Item, Update::EntitySubtype::Coin},
                {Update::EntityType::Item, Update::EntitySubtype::Carrot},
                {Update::EntityType::Item, Update::EntitySubtype::Light},
                {Update::EntityType::Item, Update::EntitySubtype::Power},
                {Update::EntityType::Item, Update::EntitySubtype::Heavy}};
        int id = 100;
        int position = 10;
        for (auto entity : entities) {
            all_updates.emplace_back(Update::Update_new::create_create_entity(
                id, entity.first, entity.second));
            all_updates.emplace_back(
                Update::Update_new::create_position(id, position, 0));
            id++;
            position += 10;
        }
        paso = true;
    }

    for (Update::Update_new update : all_updates) {
        gameState.handleUpdate(update, currentTick);
        // updatableGameState.handleUpdate(update, currentTick);
    }
}

void GuiLoop::runRenderer() {
    // Genero un nuevo estado apto para que lo consuma el renderer
    // GameStateRenderer gameStateRenderer =
    //    updatableGameState.getStateRenderer(currentTick);

    if (render == nullptr)
        throw std::runtime_error(
            "Se debe inicializar el render antes de usarlo");

    // render->presentGame(gameStateRenderer, mapInfo);
    render->presentGame2(gameState, map);
    // std::cout << "tick: " << tick_actual << "\n";
    // std::cout << "(" << gameStateRenderer.mainPlayer.position.x
    // << ", "
    //           << gameStateRenderer.mainPlayer.position.y << ")"
    //           << std::endl;
}