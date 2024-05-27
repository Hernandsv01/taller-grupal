#include "../render.h"
#include "../estado_juego.h"

#define XPICPIX 134
#define RUNSPEED 10
//Jazz run dimension (134, 34) pix

int main() {
    int width = 640;
    int height = 480;
    int pjPosX = 0;
    int pjPosY = height/2;
    std::vector<Estado> personajeEstados = {Corriendo};

    EstadoJugador personajeJazz;
    personajeJazz.posicion = Posicion{pjPosX, pjPosY};
    personajeJazz.direccion = Direccion::Derecha;
    personajeJazz.tipoPersonaje = Jazz;
    personajeJazz.puntosDeVida = 10;
    personajeJazz.estados = personajeEstados;
    personajeJazz.puntaje = 0;

    
    Render render(width, height);

    int finalPosX = width- XPICPIX;
    while (pjPosX <= finalPosX) {
        
        render.presentPlayer(personajeJazz);
        //Simulate 30 fps, 33.333 milliseconds
		render.sleep(33);
	}

	// Here all resources are automatically released and library deinitialized
	return 0;
}