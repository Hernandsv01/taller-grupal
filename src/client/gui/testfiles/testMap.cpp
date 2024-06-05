#include "../render.h"
#include "../estado_juego.h"
#include <SDL2pp/SDL2pp.hh>

//Map dimensions (768, 1024)
//Terrain parts (32, 64)
//Background (256, 256)

int main() {
    int width = 640;
    int height = 480;
    
    SDL sdl(SDL_INIT_VIDEO);
    Window window("SDL2pp demo",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			640, 480,
			SDL_WINDOW_RESIZABLE);
    Render render(window);

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

    while (1) {
        
        render.copyMap(mapInfo);
        render.presentImage();
        //Simulate 30 fps, 33.333 milliseconds
		render.sleep(50);
	}

	// Here all resources are automatically released and library deinitialized
	return 0;
}
