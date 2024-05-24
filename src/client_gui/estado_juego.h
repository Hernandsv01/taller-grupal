#ifndef ESTADO_JUEGO_H
#define ESTADO_JUEGO_H

#include <cstdint>
#include <vector>

typedef uint8_t Id;
typedef uint8_t PuntosVida;
typedef uint8_t Puntaje;

struct EstadoJugador;
struct Proyectil;
struct Enemigo;
struct Item;

struct Posicion {
    float x;
    float y;
};
enum Direccion { Izquierda, Derecha };

struct EstadoJuego {
    std::vector<EstadoJugador> jugadores;
    std::vector<Proyectil> proyectiles;
    std::vector<Enemigo> enemigos;
    std::vector<Item> items;
};

struct Entidad {
    Id id;
    Posicion posicion = Posicion{0, 0};
    Direccion direccion = Direccion::Derecha;
};

enum Estado {
    Disparar,
    Saltando,
    Cayendo,
    Corriendo,
    Intoxicado,
    Muerto,
    RecibiendoDanio
};

enum TipoPersonaje { Jazz, Spaz, Lori };
struct EstadoJugador : public Entidad {
    TipoPersonaje tipoPersonaje;
    PuntosVida puntosDeVida;
    std::vector<Estado> estados;
    Puntaje puntaje = 0;
    // Faltaría municion y tipo de arma.
    // Tendría que ver que variaciones tendría eso
};

struct Proyectil : public Entidad {
    // Faltaría tipo de proyectil
};

enum TipoEnemigo { enemigo1, enemigo2, enemigo3 };
struct Enemigo : public Entidad {
    TipoEnemigo tipoEnemigo;
};

enum TipoItem { Moneda, Arma };
struct Item : public Entidad {
    TipoItem tipoItem;
};

#endif
