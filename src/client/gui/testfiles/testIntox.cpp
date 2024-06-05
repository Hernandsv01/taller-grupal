#include "../render.h"
#include "../estado_juego.h"
#include <SDL2pp/SDL2pp.hh>

#define XPICPIX 134
#define RUNSPEED 10

int main() {
    int width = 640;
    int height = 480;

    //Map
    std::vector<Posicion> positionGround;
    for (int i= 0; i<640; i+= 32) {
        positionGround.push_back(Posicion{i, 300});
    }
    std::vector<Posicion> positionUnder;
    for (int i= 0; i<640; i+= 32) {
        positionUnder.push_back(Posicion{i, 364});
    }
    MapInfo mapInfo;
    mapInfo.mapTexture = Diamond;
    mapInfo.typeOfGround = RightSign;
    mapInfo.groundPosition = positionGround;
    mapInfo.typeOfUnder = RedDiamond;
    mapInfo.underPosition = positionUnder;


    //Initialize SDL structures
    SDL sdl(SDL_INIT_VIDEO);
    Window window("SDL2pp demo",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height,
			SDL_WINDOW_RESIZABLE);
    Render render(window);

    //Player position
    int pjPosX = 640/2;
    int pjPosY = 300;
    int cantMaxFrames = 50;

    for(int i=0; i<cantMaxFrames; i++) {
        std::vector<Estado> personajeEstados = {Intoxicado};

        EstadoJugador personajeJazz;
        personajeJazz.posicion = Posicion{pjPosX, pjPosY};
        personajeJazz.direccion = Direccion::Derecha;
        personajeJazz.tipoPersonaje = Jazz;
        personajeJazz.puntosDeVida = 10;
        personajeJazz.estados = personajeEstados;
        personajeJazz.puntaje = 0;

        //Game status
        EstadoJuegoRenderer gameCondition;
        gameCondition.jugadorPrincipal = personajeJazz;

        render.presentGame(gameCondition, mapInfo);
        //Simulate 30 fps, 33.333 milliseconds
		render.sleep(33);
    }

    while (pjPosX <= width) {
        //Main player
        std::vector<Estado> personajeEstados = {IntoxCaminando};

        EstadoJugador personajeJazz;
        personajeJazz.posicion = Posicion{pjPosX, pjPosY};
        personajeJazz.direccion = Direccion::Derecha;
        personajeJazz.tipoPersonaje = Jazz;
        personajeJazz.puntosDeVida = 10;
        personajeJazz.estados = personajeEstados;
        personajeJazz.puntaje = 0;

        //Game status
        EstadoJuegoRenderer gameCondition;
        gameCondition.jugadorPrincipal = personajeJazz;

        render.presentGame(gameCondition, mapInfo);
        pjPosX += 10;
        //Simulate 30 fps, 33.333 milliseconds
		render.sleep(33);
	}

	// Here all resources are automatically released and library deinitialized
	return 0;
}