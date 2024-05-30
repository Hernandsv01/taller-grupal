#ifndef MAP_H
#define MAP_H

#include <cstdint>
#include <string>
#include <vector>

// Campaz cambiamos el nombre de los distintos triangulos
enum BlockColission {
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

struct Block {
    Coordinate coordinate;
    BlockColission colission;
};

typedef std::string IdTexture;

struct BlockTexture {
    Coordinate coordinate;
    IdTexture texture;
};

class Map {
    // Servidor
   public:
    std::vector<Block> get_all_blocks_colissions();
    BlockColission get_block_collision(Coordinate coordenadas);
    BlockColission get_block_collision(uint8_t x, uint8_t y);

    std::vector<Block> get_player_spawns();
    std::vector<Block> get_enemy_spawns();
    std::vector<Block> get_items_spawns();

    // Cliente
    std::vector<BlockTexture> get_all_block_textures();
    IdTexture get_background();
    Coordinate get_map_size();

    // All

    Coordinate get_map_size();
};

#endif
