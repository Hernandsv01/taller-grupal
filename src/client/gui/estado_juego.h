#ifndef ESTADO_JUEGO_H
#define ESTADO_JUEGO_H

#include <algorithm>
#include <cstdint>
#include <map>
#include <vector>

#include "../../common/Update.h"

#define FACTOR_TAMANIO 5

typedef uint8_t Id_t;
typedef uint8_t HealthPoints_t;
typedef uint8_t Score_t;

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

// Representacion de una entidad generica.
struct Entity {
    Id_t id;
    Position position{0, 0};
    enums_value_update::Direction direction =
        enums_value_update::Direction::Right;
};

struct State {
   private:
    uint32_t start_tick = 0;

   public:
    enums_value_update::Player_State_Enum state =
        enums_value_update::Player_State_Enum::Idle;
    uint32_t current_tick = 0;
    // Tiempo que lleva en ese estado.

    void updateTick(uint32_t tick) {
        this->current_tick = tick - this->start_tick;
    }

    bool operator==(const enums_value_update::Player_State_Enum &stateEnum) {
        return this->state == stateEnum;
    }

    State(enums_value_update::Player_State_Enum state, uint32_t tick)
        : state(state), start_tick(tick) {}
};

enum CharacterType { Jazz, Spaz, Lori };
struct PlayerState : public Entity {
    CharacterType characterType;
    State state = State(enums_value_update::Player_State_Enum::Idle, 0);
    HealthPoints_t healthPoints = 10;
    Score_t score = 0;
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
    bool matchEnded;
    std::map<Id_t, PlayerState> players;
    std::map<Id_t, Projectile> projectiles;
    std::map<Id_t, Enemy> enemies;
    std::map<Id_t, Item> items;
    Id_t mainPlayerId = 1;

   private:
    //    https://stackoverflow.com/questions/771453/copy-map-values-to-vector-in-stl
    // Convierte un map a un vector, elimnando los ids. Lo creo como un template
    // para poder usar el mismo codigo para las distintas entidades.
    template <typename T>
    std::vector<T> get_vector_from(std::map<Id_t, T> map) {
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

    PlayerState &getPlayerStateReferenceById(Id_t id) {
        return this->players.at(id);
    }

    Entity &getEntityReferenceById(Id_t id) {
        if (this->players.count(id)) {
            return this->players.at(id);
        } else if (this->projectiles.count(id)) {
            return this->projectiles.at(id);
        } else if (this->enemies.count(id)) {
            return this->enemies.at(id);
        } else if (this->items.count(id)) {
            return this->items.at(id);
        }

        // No se encontró la ID. Devuelvo algun tipo de error;
        return this->items.at(id);
    }

    void addPlayer(PlayerState player) {
        this->players.insert({player.id, player});
    }

    void addProjectile(Projectile projectile) {
        this->projectiles.insert({projectile.id, projectile});
    }

    void addEnemy(Enemy enemy) { this->enemies.insert({enemy.id, enemy}); }

    void addItem(Item item) { this->items.insert({item.id, item}); }

    void deleteEntity(Id_t id) {
        if (id == this->mainPlayerId) {
            // No se puede borrar al jugador principal.
            throw std::runtime_error(
                "No se puede borrar al jugador principal.");
        }

        if (this->players.count(id)) {
            this->players.erase(id);
        } else if (this->projectiles.count(id)) {
            this->projectiles.erase(id);
        } else if (this->enemies.count(id)) {
            this->enemies.erase(id);
        } else if (this->items.count(id)) {
            this->items.erase(id);
        }

        throw std::runtime_error("No se encontro la entidad con el id dado.");
    }

    uint8_t getLocalSubtypeOfEntity(
        Update::EntityTypeAndSubtype entityTypeAndSubtype) {
        Update::EntityType entity_type_value = entityTypeAndSubtype.type;
        Update::EntitySubtype entity_subtype_value =
            entityTypeAndSubtype.subtype;
        switch (entity_type_value) {
            case Update::Player:
                switch (entity_subtype_value) {
                    case Update::Jazz:
                        return static_cast<uint8_t>(CharacterType::Jazz);
                    case Update::Spaz:
                        return static_cast<uint8_t>(CharacterType::Spaz);
                    case Update::Lori:
                        return static_cast<uint8_t>(CharacterType::Lori);
                }
            case Update::Bullet:
                return 0;
            case Update::Enemy:
                switch (entity_subtype_value) {
                    case Update::Enemy1:
                        return static_cast<uint8_t>(EnemyType::Enemy1);
                    case Update::Enemy2:
                        return static_cast<uint8_t>(EnemyType::Enemy2);
                    case Update::Enemy3:
                        return static_cast<uint8_t>(EnemyType::Enemy3);
                }
            case Update::Item:
                switch (entity_subtype_value) {
                    case Update::Coin:
                        return static_cast<uint8_t>(ItemType::Coin);
                    case Update::Weapon:
                        return static_cast<uint8_t>(ItemType::Weapon);
                }

            default:
                return 0;
        }
    }

    void addEntity(Id_t id, Update::EntityTypeAndSubtype entityTypeAndSubtype) {
        switch (entityTypeAndSubtype.type) {
            case Update::Player: {
                PlayerState player{id};
                player.characterType = static_cast<CharacterType>(
                    getLocalSubtypeOfEntity(entityTypeAndSubtype));

                this->addPlayer(player);
                break;
            }
            case Update::Bullet: {
                Projectile projectile{id};
                this->addProjectile(projectile);
                break;
            }
            case Update::Enemy: {
                Enemy enemy{id};
                enemy.enemyType = static_cast<EnemyType>(
                    getLocalSubtypeOfEntity(entityTypeAndSubtype));
                this->addEnemy(enemy);
                break;
            }
            case Update::Item: {
                Item item{id};
                item.itemType = static_cast<ItemType>(
                    getLocalSubtypeOfEntity(entityTypeAndSubtype));
                this->addItem(item);
                break;
            }
        }
    }

    Position conversionTamanio(Update::PositionFloat pos) {
        return {static_cast<int>(pos.x * FACTOR_TAMANIO),
                static_cast<int>(pos.y * FACTOR_TAMANIO)};
    }

    void handleUpdate(Update::Update_new update, uint32_t tick) {
        switch (update.get_update_type()) {
            case Update::CreateEntity: {
                Update::EntityTypeAndSubtype entity_type_and_subtype =
                    update.get_entity_type_and_subtype();

                this->addEntity(update.get_id(), entity_type_and_subtype);

                break;
            }
            case Update::Position: {
                Entity &entity = this->getEntityReferenceById(update.get_id());
                entity.position = conversionTamanio(update.get_position());
                break;
            }
            case Update::Direction: {
                PlayerState &player = this->players.at(update.get_id());
                player.direction = static_cast<enums_value_update::Direction>(
                    update.get_value());
                break;
            }
            case Update::State: {
                PlayerState &player = this->players.at(update.get_id());
                player.state =
                    State(static_cast<enums_value_update::Player_State_Enum>(
                              update.get_value()),
                          tick);
                break;
            }
            case Update::Health: {
                PlayerState &player = this->players.at(update.get_id());
                player.healthPoints = update.get_value();
                break;
            }
            case Update::Score: {
                PlayerState &player = this->players.at(update.get_id());
                player.score = update.get_value();
                break;
            }
            case Update::DeleteEntity: {
                this->deleteEntity(update.get_id());
                break;
            }
            case Update::MatchEnded: {
                this->matchEnded = true;
                break;
            }
        }
    }

    // Devuelve el estado del juego en el formato para que le sea mas
    // sencillo al renderer.
    GameStateRenderer getStateRenderer(uint32_t tick) {
        GameStateRenderer state;

        PlayerState mainPlayer = this->players.at(mainPlayerId);
        mainPlayer.state.updateTick(tick);

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

        for (PlayerState &player : state.players) {
            player.state.updateTick(tick);
        }

        return state;
    }

    bool hasMatchEnded() { return this->matchEnded; }
};

#endif
