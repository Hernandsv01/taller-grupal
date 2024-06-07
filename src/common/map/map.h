#ifndef MAP_H
#define MAP_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "yaml-cpp/yaml.h"

typedef uint8_t MapId;

// Campaz cambiamos el nombre de los distintos triangulos
// Tal vex tambien incluya si es un spawn de enemigo/jugador/item
enum Collision : uint8_t {
    Air,                 // ▢
    Cube,                // ■
    TriangleLowerRight,  // ◢
    TriangleLowerLeft,   // ◣
    TriangleUpperLeft,   // ◤
    TriangleUpperRight   // ◥
};

// Ubicacion del bloque. Como es uint8_t, el mapa puede tener hasta 255x255
// bloques.
struct Coordinate {
    uint8_t x;
    uint8_t y;

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
    bool has_collision() { return (collision != Collision::Air); }

    bool has_texture() { return (collision != Collision::Air); }
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

    uint8_t size_x;
    uint8_t size_y;
    std::vector<std::vector<Block>> blocks;

    IdTexture background_texture = "undefined";

    Map();

   public:
    // Servidor

    // Carga un mapa desde un archivo yaml.
    Map static fromYaml(const char* path);

    Map(uint8_t size_x, uint8_t size_y);

    std::string get_name() const;

    // Devuelve todos los bloques que tengan alguna colision.
    std::vector<BlockOnlyCollision> get_all_blocks_collisions() const;

    // Devuelve la colision del bloque especificado. (Puede devolver Air)
    Collision get_block_collision(const Coordinate& coordenadas) const;
    Collision get_block_collision(uint8_t x, uint8_t y) const;

    // TODO:
    // std::vector<Coordinate> get_player_spawns() const;
    // std::vector<Coordinate> get_enemy_spawns() const;
    // std::vector<Coordinate> get_items_spawns() const;

    // Cliente

    // Devuelve todos los bloques que tengan alguna textura. (no incluye los
    // bloques Air)
    std::vector<BlockOnlyTexture> get_all_block_textures() const;
    IdTexture get_background() const;

    // All

    // Devuelve el tamaño en bloques del mapa.
    Coordinate get_map_size() const;
};

#endif
