#include "render.h"

#define BASESPRITE 0
#define JAZZSTANDX 46
#define JAZZSTANDY 49
#define STANDSPRITELONG 27

#define JAZZRUNX 67
#define JAZZRUNY 35
#define RUNSPRITELONG 8

#define GROUND 0
#define UNDER 1
#define BACK 2
#define WALL 3
#define PARTDIMX 32
#define PARTDIMY 64
#define WALLDIMENSION 256
//Jazz stand dimension (46, 49) pix
//Jazz run dimension (67, 34) pix

Render::Render(int width, int height) :
	sdl(SDL_INIT_VIDEO),
	window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE),
	renderer(window, -1, SDL_RENDERER_ACCELERATED),
	standSpritesJazz(renderer, DATA_PATH "/Jazz_stand.png"),
	runSpritesJazz(renderer, DATA_PATH "/Jazz_run.png"),
	mapsTexture(renderer, DATA_PATH "/map_diamond.png"),
	frame(0) {
}

void Render::presentGame(EstadoJuegoRenderer gameStatus, MapInfo mapInfo) {
	copyMap(mapInfo);
	copyPlayer(gameStatus.jugadorPrincipal);
	renderer.Present();
}

void Render::copyPlayer(EstadoJugador jugador) {
	int xPos = jugador.posicion.x;
	int yPos = jugador.posicion.y;
	
	if (jugador.estados[0] == Esperando) {
		copyEntity(xPos, yPos,JAZZSTANDX, JAZZSTANDY, STANDSPRITELONG, standSpritesJazz);
	} else {
		copyEntity(xPos, yPos,JAZZRUNX, JAZZRUNY, RUNSPRITELONG, runSpritesJazz);
	}
}

void Render::copyEntity(int xPos, int yPos, 
							int spriteLong, int spriteHigh,
							int animationLong, Texture &sprite) {
	
	int srcX = frame * spriteLong;

	renderer.Copy(
			sprite,
			Rect(srcX, BASESPRITE, spriteLong, spriteHigh),
			Rect(xPos, yPos, spriteLong, spriteHigh));
	
	frame = (frame +1) % animationLong;
}

void Render::copyMap(MapInfo mapInfo) {
	copyWall();
	copyMapPart(GROUND, mapInfo.typeOfGround, mapInfo.groundPosition);
	copyMapPart(UNDER, mapInfo.typeOfUnder, mapInfo.underPosition);
}

void Render::copyWall() {
	for (int i=0; i< window.GetWidth(); i += WALLDIMENSION) {
		for (int j=0; j< window.GetHeight(); j += WALLDIMENSION) {
			renderer.Copy(
						mapsTexture,
						Rect(0, WALL*PARTDIMY, WALLDIMENSION, WALLDIMENSION),
						Rect(i, j, WALLDIMENSION, WALLDIMENSION));
		}
	}
}

void Render::copyMapPart(int typeOfPart ,int part, std::vector<Posicion> positions) {
	for (auto position : positions) {
		renderer.Copy(
					mapsTexture,
					Rect(part *PARTDIMX, typeOfPart*PARTDIMY, PARTDIMX, PARTDIMY),
					Rect(position.x, position.y, PARTDIMX, PARTDIMY));
	}
}

void Render::sleep(int millisecond) {
	SDL_Delay(millisecond);
}