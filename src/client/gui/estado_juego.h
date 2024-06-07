#ifndef ESTADO_JUEGO_H
#define ESTADO_JUEGO_H

#include <algorithm>
#include <cstdint>
#include <map>
#include <vector>

#include "../../common/Update.h"

typedef uint8_t Id;
typedef uint8_t HealthPoints;
typedef uint8_t Score;

struct PlayerState;
struct Projectile;
struct Enemy;
struct Item;

struct Position {
    // La posicion estaría definida en pixeles, para que le sea más util al
    // renderer.
    int x;
    int y;
};
// Direccion a la que apunta la entidad (para saber si voltear horizontalmente
// la textura)
enum Direction { Left, Right };

// Representacion de una entidad generica.
struct Entity {
    Id id;
    Position position = Position{0, 0};
    Direction direction = Direction::Right;
};

// Posibles estados de ¿solo jugador?
enum State {
    Idle,
    Shooting,
    Jumping,
    Falling,
    Running,
    Intoxicated,
    IntoxWalking,
    Dead,
    TakingDamage
};

enum CharacterType { Jazz, Spaz, Lori };
struct PlayerState : public Entity {
    CharacterType characterType;
    HealthPoints healthPoints;
    // suponemos que un jugador podría tener más de un estado.
    std::vector<State> states;
    Score score = 0;
    // Faltaría municion y tipo de arma.
    // Tendría que ver que variaciones tendría eso
};

struct Projectile : public Entity {
    // Tenemos que averiguar que definiría un proyectil.
};
// Esta por definir los tipos de enemigos.
enum EnemyType { Enemy1, Enemy2, Enemy3 };
struct Enemy : public Entity {
    EnemyType enemyType;
};

enum ItemType { Coin, Weapon };
struct Item : public Entity {
    ItemType itemType;
};

// Representa el estado del juego que se le pasa al renderer.
struct GameStateRenderer {
    PlayerState mainPlayer;
    std::vector<PlayerState> players;
    std::vector<Projectile> projectiles;
    std::vector<Enemy> enemies;
    std::vector<Item> items;
};

enum TypeOfMaps { Diamond };
enum TypeOfGround { RightSign = 2 };
enum TypeOfUnderG { RedDiamond = 0 };
struct MapInfo {
    TypeOfMaps mapTexture;
    TypeOfGround typeOfGround;
    std::vector<Position> groundPosition;
    TypeOfUnderG typeOfUnder;
    std::vector<Position> underPosition;
};

// Clase que se encarga de mantener el estado del juego actualizado.
// Incluye todos los jugadores, proyectiles, enemigos e items, indexado por ID,
// para ser mas facil su actualizacion.
class UpdatableGameState {
    std::map<Id, PlayerState> players;
    std::map<Id, Projectile> projectiles;
    std::map<Id, Enemy> enemies;
    std::map<Id, Item> items;
    Id mainPlayerId = 1;

   private:
    //    https://stackoverflow.com/questions/771453/copy-map-values-to-vector-in-stl
    // Convierte un map a un vector, elimnando los ids. Lo creo como un template
    // para poder usar el mismo codigo para las distintas entidades.
    template <typename T>
    std::vector<T> get_vector_from(std::map<Id, T> map) {
        std::vector<T> vector;

        std::transform(
            map.begin(), map.end(), std::back_inserter(vector),
            [](const auto &pair_id_key) { return pair_id_key.second; });

        return vector;
    }

   public:
    // Agrega un jugador principal al estado del juego
    void addMainPlayer(PlayerState player) {
        this->mainPlayerId = player.id;
        this->addPlayer(player);
    }

    void addPlayer(PlayerState player) {
        this->players.insert({player.id, player});
    }

    void handleUpdate(Update update) {
        // Incluir logica de que tipo de update es
        if (players.count(update.id)) {
            players[update.id].position.x = update.value;
        }
    }

    // Devuelve el estado del juego en el formato para que le sea mas sencillo
    // al renderer.
    GameStateRenderer getStateRenderer() {
        GameStateRenderer state;

        PlayerState mainPlayer = this->players.at(mainPlayerId);

        state.mainPlayer = mainPlayer;

        // Elimino jugador principal del map
        this->players.erase(mainPlayerId);

        // Construyo el vector de jugadores (no principales)
        state.players = this->get_vector_from(this->players);

        // vuelvo a agregar jugador principal a map
        this->players[mainPlayerId] = mainPlayer;

        state.items = this->get_vector_from(this->items);
        state.enemies = this->get_vector_from(this->enemies);
        state.projectiles = this->get_vector_from(this->projectiles);

        return state;
    }
};

#endif
