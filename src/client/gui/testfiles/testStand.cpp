#include "../render.h"
#include "../estado_juego.h"
#include <SDL2pp/SDL2pp.hh>


int main() {
    int width = 640;
    int height = 480;
    int pjPosX = width/2;
    int pjPosY = 300;
    int groundPosY = 300;

    //Map
    std::vector<Posicion> positionGround;
    for (int i= 0; i<640; i+= 32) {
        positionGround.push_back(Posicion{i, groundPosY});
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

    //Jugador principal
    std::vector<Estado> playerState = {Esperando};

    EstadoJugador mainPlayer;
    mainPlayer.tipoPersonaje = Jazz;
    mainPlayer.puntosDeVida = 10;
    mainPlayer.estados = playerState;
    mainPlayer.puntaje = 0;
    mainPlayer.posicion = Posicion{pjPosX, pjPosY};
    mainPlayer.direccion = Direccion::Derecha;

    //Estado del juego
    EstadoJuegoRenderer gameCondition;
    gameCondition.jugadorPrincipal = mainPlayer;

    //Initialize SDL structures
    SDL sdl(SDL_INIT_VIDEO);
    Window window("SDL2pp demo",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			640, 480,
			SDL_WINDOW_RESIZABLE);
    Render render(window);

    while (1) {
        
        render.presentGame(gameCondition, mapInfo);

        //Simulate 30 fps, 33.333 milliseconds
		render.sleep(34);
	}

	// Here all resources are automatically released and library deinitialized
	return 0;
}