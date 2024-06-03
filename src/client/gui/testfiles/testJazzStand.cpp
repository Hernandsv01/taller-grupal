#include "../render.h"
#include "../estado_juego.h"

int main() {
    int width = 640;
    int height = 480;
    int pjPosX = width/2;
    int pjPosY = height/2;
    std::vector<Estado> personajeEstados = {Parado};

    EstadoJugador personajeJazz;
    personajeJazz.posicion = Posicion{pjPosX, pjPosY};
    personajeJazz.direccion = Direccion::Derecha;
    personajeJazz.tipoPersonaje = Jazz;
    personajeJazz.puntosDeVida = 10;
    personajeJazz.estados = personajeEstados;
    personajeJazz.puntaje = 0;

    Window window("DEMO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                  width, height, SDL_WINDOW_RESIZABLE);

    Render render(window);

    while (1) {
        render.presentPlayer(personajeJazz);

        //Simulate 30 fps, 33.333 milliseconds
        render.sleep(34);
    }

    // Here all resources are automatically released and library deinitialized
    return 0;
}