#ifndef MAP_H
#define MAP_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>

typedef uint8_t MapId;

// Campaz cambiamos el nombre de los distintos triangulos
enum BlockCollision : uint8_t {
    Air,                 // ▢
    Cube,                // ■
    TriangleLowerRight,  // ◢
    TriangleLowerLeft,   // ◣
    TriangleUpperLeft,   // ◤
    TriangleUpperRight   // ◥
};

struct Coordinate {
    uint8_t x;
    uint8_t y;
};

typedef std::string IdTexture;

struct Block {
    BlockCollision collision;
    IdTexture texture;
};

struct BlockOnlyTexture {
    Coordinate coordinate;
    IdTexture texture;
};

struct BlockOnlyCollision {
    Coordinate coordinate;
    BlockCollision collision;
};

struct BlockWithCoordinate {
    Coordinate coordinate;
    BlockCollision collision;
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

   public:
    std::string map_name = "unnamed";

    uint8_t size_x;
    uint8_t size_y;
    std::vector<std::vector<Block>> bloques;

    IdTexture background_texture = "undefined";

    // Servidor
   public:
    explicit Map(const char* path);

    Map();

    Map(uint8_t size_x, uint8_t size_y);

    std::vector<BlockOnlyCollision> get_all_blocks_collisions();
    BlockCollision get_block_collision(Coordinate coordenadas);
    BlockCollision get_block_collision(uint8_t x, uint8_t y);

    std::vector<Coordinate> get_player_spawns();
    std::vector<Coordinate> get_enemy_spawns();
    std::vector<Coordinate> get_items_spawns();

    // Cliente
    std::vector<BlockOnlyTexture> get_all_block_textures();
    IdTexture get_background();

    // All

    Coordinate get_map_size();
};

#endif
