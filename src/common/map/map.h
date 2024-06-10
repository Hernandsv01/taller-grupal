#ifndef MAP_H
#define MAP_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "outside_map_limits_error.h"
#include "yaml-cpp/yaml.h"

typedef uint8_t MapId;
typedef uint8_t coord_unit;

// Campaz cambiamos el nombre de los distintos triangulos
// Tal vex tambien incluya si es un spawn de enemigo/jugador/item
enum Collision : uint8_t {
    Air,                 // ▢
    Cube,                // ■
    TriangleLowerRight,  // ◢
    TriangleLowerLeft,   // ◣
    TriangleUpperLeft,   // ◤
    TriangleUpperRight,  // ◥
    ItemSpawn,
    PlayerSpawn,
    EnemySpawn
};

// Ubicacion del bloque. Como es uint8_t, el mapa puede tener hasta 255x255
// bloques.
struct Coordinate {
    coord_unit x;
    coord_unit y;

    bool operator==(const Coordinate& other) const {
        return (other.x == this->x) && (other.y == this->y);
    }
};

// Por ahora el ID de una textura es un string. Tal vez eso cambie por otra
// cosa.
typedef std::string IdTexture;

struct Block {
    Collision collision;
    IdTexture texture;

    // Por ahora uso estas definiciones de si tiene textura y colision
    bool has_collision() const {
        return (collision != Collision::Air &&
                collision != Collision::ItemSpawn &&
                collision != Collision::PlayerSpawn &&
                collision != Collision::EnemySpawn);
    }

    bool has_texture() const {
        return (collision != Collision::Air &&
                collision != Collision::ItemSpawn &&
                collision != Collision::PlayerSpawn &&
                collision != Collision::EnemySpawn);
    }
};

// Para cuando obtengo todos los bloques con textura
struct BlockOnlyTexture {
    Coordinate coordinate;
    IdTexture texture;

    bool operator==(const BlockOnlyTexture& other) const {
        return (this->coordinate == other.coordinate) &&
               (this->texture == other.texture);
    }
};

// Para cuando obtengo todos los bloques con colision
struct BlockOnlyCollision {
    Coordinate coordinate;
    Collision collision;

    bool operator==(const BlockOnlyCollision& other) const {
        return (this->coordinate == other.coordinate) &&
               (this->collision == other.collision);
    }
};

// Para cuando obtengo todos los bloques (todavía sin uso)
struct BlockWithCoordinate {
    Coordinate coordinate;
    Collision collision;
    IdTexture texture;

    operator BlockOnlyTexture() const {
        return BlockOnlyTexture{coordinate, texture};
    };

    operator BlockOnlyCollision() const {
        return BlockOnlyCollision{coordinate, collision};
    };

    operator Block() const { return Block{collision, texture}; };
};

class Map {
    MapId id = 1;

    // Para que al crearse un mapa a partir de un nodo,
    // se pueda inicializar el objeto mapa vacío (lo requiere la api de
    // yaml-cpp)
    friend Map YAML::as_if<Map, void>::operator()() const;

    // Para poder completar las propiedades privadas del mapa, ya que el mapa se
    // inicializa vacío, porque así lo requiera la api de yaml-cpp
    friend struct YAML::convert<Map>;

    std::string map_name = "unnamed";

    coord_unit size_x;
    coord_unit size_y;
    std::vector<std::vector<Block>> blocks;

    IdTexture background_texture = "undefined";

    Map();

    template <typename T>
    std::vector<T> get_blocks_with_condition_and_constructor(
        bool (*condition)(const Block&),
        T (*constructor)(const Coordinate coordinate, const Block&)) const;

    std::vector<Coordinate> get_coord_of_blocks_with_condition(
        bool (*condition)(const Block&)) const;

   public:
    // Servidor

    // Carga un mapa desde un archivo yaml.
    Map static fromYaml(const char* path);

    Map(coord_unit size_x, coord_unit size_y);

    std::string get_name() const;

    // Devuelve todos los bloques que tengan alguna colision.
    std::vector<BlockOnlyCollision> get_all_blocks_collisions() const;

    // Devuelve la colision del bloque especificado. (Puede devolver Air)
    Collision get_block_collision(const Coordinate& coordenadas) const;
    Collision get_block_collision(coord_unit x, coord_unit y) const;

    std::vector<Coordinate> get_player_spawns() const;
    std::vector<Coordinate> get_enemy_spawns() const;
    std::vector<Coordinate> get_items_spawns() const;

    // Cliente

    // Devuelve todos los bloques que tengan alguna textura. (no incluye los
    // bloques Air y de spawn)
    std::vector<BlockOnlyTexture> get_all_block_textures() const;
    IdTexture get_background() const;

    // All

    // Devuelve el tamaño en bloques del mapa.
    Coordinate get_map_size() const;

    // Editor mapa
    void add_block(const Coordinate& coordinate, const Block& block);
};

#endif
