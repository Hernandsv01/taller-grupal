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
    // Modifico a "int", son los pixeles
    int x;
    int y;
};
enum Direction { Left, Right };

struct Entity {
    Id id;
    Position position = Position{0, 0};
    Direction direction = Direction::Right;
};

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
    std::vector<State> states;
    Score score = 0;
    // Faltaría municion y tipo de arma.
    // Tendría que ver que variaciones tendría eso
};

struct GameStateRenderer {
    PlayerState mainPlayer;
    std::vector<PlayerState> players;
    std::vector<Projectile> projectiles;
    std::vector<Enemy> enemies;
    std::vector<Item> items;
};

struct Projectile : public Entity {
    // Faltaría tipo de proyectil
};

enum enemyType { Enemy1, Enemy2, Enemy3 };
struct Enemy : public Entity {
    enemyType enemyType;
};

enum ItemType { Coin, Weapon };
struct Item : public Entity {
    ItemType itemType;
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

class EstadoJuegoActualizable {
    std::map<Id, PlayerState> players;
    std::map<Id, Projectile> projectiles;
    std::map<Id, Enemy> enemies;
    std::map<Id, Item> items;
    Id mainPlayerId = 1;

   private:
    //    https://stackoverflow.com/questions/771453/copy-map-values-to-vector-in-stl
    template <typename T>
    std::vector<T> get_vector_from(std::map<Id, T> map) {
        std::vector<T> vector;

        std::transform(
            map.begin(), map.end(), std::back_inserter(vector),
            [](const auto &pair_id_key) { return pair_id_key.second; });

        return vector;
    }

   public:
    void addMainPlayer(PlayerState player) {
        this->mainPlayerId = player.id;
        this->players.insert({player.id, player});
    }

    void actualizar(Update update) {
        // Incluir logica de que tipo de update es
        if (players.count(update.id)) {
            players[update.id].position.x = update.value;
        }
    }

    GameStateRenderer getStateRenderer() {
        GameStateRenderer state;

        PlayerState mainPlayer = this->players.at(mainPlayerId);

        state.mainPlayer = mainPlayer;

        // Elimino jugador principal de map
        this->players.erase(mainPlayerId);

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
