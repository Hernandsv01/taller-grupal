#ifndef ESTADO_JUEGO_H
#define ESTADO_JUEGO_H

#include <algorithm>
#include <cstdint>
#include <map>
#include <vector>

#include "../../common/Update.h"

typedef uint8_t Id;
typedef uint8_t PuntosVida;
typedef uint8_t Puntaje;

struct EstadoJugador;
struct Proyectil;
struct Enemigo;
struct Item;

struct Posicion {
    // Modifico a "int", son los pixeles
    int x;
    int y;
};
enum Direccion { Izquierda, Derecha };

struct Entidad {
    Id id;
    Posicion posicion = Posicion{0, 0};
    Direccion direccion = Direccion::Derecha;
};

enum Estado {
    Parado,
    Disparando,
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

struct EstadoJuegoRenderer {
    EstadoJugador jugadorPrincipal;
    std::vector<EstadoJugador> jugadores;
    std::vector<Proyectil> proyectiles;
    std::vector<Enemigo> enemigos;
    std::vector<Item> items;
};

class EstadoJuegoActualizable {
    std::map<Id, EstadoJugador> jugadores;
    std::map<Id, Proyectil> proyectiles;
    std::map<Id, Enemigo> enemigos;
    std::map<Id, Item> items;
    Id id_jugador_principal = 1;

   private:
    //    https://stackoverflow.com/questions/771453/copy-map-values-to-vector-in-stl
    template <typename T>
    std::vector<T> get_vector_from(std::map<Id, T> map) {
        std::vector<T> vector;

        std::transform(
            map.begin(), map.end(), std::back_inserter(vector),
            [](const auto &par_id_key) { return par_id_key.second; });

        return vector;
    }

   public:
    void agregar_jugador_principal(EstadoJugador jugador) {
        this->id_jugador_principal = jugador.id;
        this->jugadores.insert({jugador.id, jugador});
    }

    void actualizar(Update update) {
        // Incluir logica de que tipo de update es
        if (jugadores.count(update.id)) {
            jugadores[update.id].posicion.x = update.value;
        }
    }

    EstadoJuegoRenderer obtener_estado() {
        EstadoJuegoRenderer estado;

        EstadoJugador jugador_principal =
            this->jugadores.at(id_jugador_principal);

        estado.jugadorPrincipal = jugador_principal;

        // Elimino jugador principal de map
        this->jugadores.erase(id_jugador_principal);

        estado.jugadores = this->get_vector_from(this->jugadores);

        // vuelvo a agregar jugador principal a map
        this->jugadores[id_jugador_principal] = jugador_principal;

        estado.items = this->get_vector_from(this->items);
        estado.enemigos = this->get_vector_from(this->enemigos);
        estado.proyectiles = this->get_vector_from(this->proyectiles);

        return estado;
    }
};

#endif
