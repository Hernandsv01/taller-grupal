#include "../estado_juego.h"
#include "../render.h"

int main() {
    int width = 640;
    int height = 480;
    int pjPosX = width / 2;
    int pjPosY = height / 2;
    std::vector<State> personajeEstados = {Idle};

    PlayerState personajeJazz;
    personajeJazz.position = Position{pjPosX, pjPosY};
    personajeJazz.direction = Direction::Right;
    personajeJazz.characterType = Jazz;
    personajeJazz.healthPoints = 10;
    personajeJazz.states = personajeEstados;
    personajeJazz.score = 0;

    Window window("DEMO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                  width, height, SDL_WINDOW_RESIZABLE);

    Render render(window);

    while (1) {
        render.presentPlayer(personajeJazz);

        // Simulate 30 fps, 33.333 milliseconds
        render.sleep(34);
    }

    // Here all resources are automatically released and library deinitialized
    return 0;
}