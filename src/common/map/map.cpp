#include "map.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "yaml-cpp/yaml.h"

// Para que no aparezca error de MAP_PATH no definido.
// Esta definido en el cmake
#ifndef MAP_PATH
#define MAP_PATH "./"
#endif

namespace YAML {
template <>
struct convert<::Map> {
    // Convierte un objeto Map a un Nodo.
    static Node encode(const ::Map& map) {
        Node node;

        node["name"] = map.map_name;
        node["size"]["x"] = static_cast<uint>(map.size_x);
        node["size"]["y"] = static_cast<uint>(map.size_y);

        node["texture_background"] = map.background_texture;

        // Esta linea eventualmente llama al encodeo de Block
        node["blocks"] = map.blocks;

        return node;
    }

    // Convierte un Nodo a un objeto Map.
    static bool decode(const Node& node, ::Map& map) {
        map.map_name = node["name"].as<std::string>();

        map.size_x = static_cast<coord_unit>(node["size"]["x"].as<uint>());
        map.size_y = static_cast<coord_unit>(node["size"]["y"].as<uint>());

        map.background_texture = node["texture_background"].as<std::string>();

        // Esta linea eventualmente llama al decodeo de Block
        map.blocks = node["blocks"].as<std::vector<std::vector<::Block>>>();

        return true;
    }
};

template <>
struct convert<::Block> {
    // Convierte un objeto Block a un Nodo.
    static Node encode(const ::Block& block) {
        Node node;
        // Esta linea eventualmente llama al encodeo de Collision
        node.push_back(block.collision);

        node.push_back(block.texture);
        return node;
    }

    // Convierte un Nodo a un objeto Block.
    static bool decode(const Node& node, ::Block& block) {
        if (!node.IsSequence() || node.size() != 2) {
            return false;
        }

        // Esta linea eventualmente llama al decodeo de Collision
        block.collision = node[0].as<::Collision>();

        block.texture = node[1].as<::IdTexture>();
        return true;
    }
};

template <>
struct convert<::Collision> {
    // Convierte un objeto Collision a un Nodo.
    static Node encode(const ::Collision& blockCollision) {
        // tengo que usar uint porque sino por alguna razon
        // crea el nodo como un string de bytes, en vez de un numero
        uint number_collision = (uint)blockCollision;
        Node node(number_collision);
        return node;
    }

    // Convierte un Nodo a un objeto Collision.
    static bool decode(const Node& node, ::Collision& block) {
        if (!node.IsScalar()) return false;

        // Como comente antes, tengo que convertirlo a un uint,
        // porque sino hace cosas raras
        block = static_cast<Collision>(node.as<uint>());

        return true;
    }
};
}  // namespace YAML

Map::Map() {}

Map Map::fromYaml(std::string file_name) {
    std::string file_path = MAP_PATH;
    file_path += file_name;

    if (file_path.substr(file_path.size() - 5) != ".yaml") {
        file_path += ".yaml";
    }

    if (!std::filesystem::exists(file_path))
        throw std::filesystem::filesystem_error("No existe tal mapa",
                                                std::error_code());

    YAML::Node nodo_mapa = YAML::LoadFile(file_path);

    // Yaml me devuelve una instancia nueva de mapa.
    // No encontré como hacer que el constructor devuelva eso,
    // así que hice el metodo estatico.
    Map mapa_temp = nodo_mapa.as<Map>();

    return mapa_temp;
}

void Map::toYaml() const {
    YAML::Node nodo_mapa = YAML::convert<Map>::encode(*this);

    // MAP_PATH definido por cmake
    std::string path = MAP_PATH + this->map_name + ".yaml";

    std::ofstream archivo(path);

    std::cout << archivo.is_open() << ":" << path << std::endl;

    archivo << nodo_mapa;
}

Map::Map(coord_unit size_x, coord_unit size_y)
    : size_x(size_x),
      size_y(size_y),
      blocks(size_y, std::vector<::Block>(size_x)) {}

std::string Map::get_name() const { return this->map_name; }

void Map::set_name(const std::string& name) { this->map_name = name; }

std::vector<BlockOnlyCollision> Map::get_all_blocks_collisions() const {
    return get_blocks_with_condition_and_constructor<BlockOnlyCollision>(
        [](const Block& block) { return block.has_collision(); },
        [](const Coordinate coordinate, const Block& block) {
            return BlockOnlyCollision{coordinate, block.collision};
        });
}

Collision Map::get_block_collision(const Coordinate& coord) const {
    return Map::get_block_collision(coord.x, coord.y);
}

Collision Map::get_block_collision(coord_unit x, coord_unit y) const {
    if (x >= size_x || y >= size_y) {
        throw OutsideMapLimitsError();
    }

    return blocks[y][x].collision;
}

template <typename T>
std::vector<T> Map::get_blocks_with_condition_and_constructor(
    bool (*condition)(const Block&),
    T (*constructor)(const Coordinate, const Block&)) const {
    std::vector<T> coords;

    for (coord_unit y = 0; y < size_y; y++) {
        for (coord_unit x = 0; x < size_x; x++) {
            Block current_block = blocks[y][x];

            if (condition(current_block)) {
                coords.emplace_back(
                    constructor(Coordinate{x, y}, current_block));
            }
        }
    }

    return coords;
}

std::vector<Coordinate> Map::get_coord_of_blocks_with_condition(
    bool (*condition)(const Block&)) const {
    return get_blocks_with_condition_and_constructor<Coordinate>(
        condition, [](const Coordinate coordinate, const Block& block) {
            return coordinate;
        });
}

std::vector<Coordinate> Map::get_player_spawns() const {
    return get_coord_of_blocks_with_condition([](const Block& block) {
        return block.collision == Collision::PlayerSpawn;
    });
}
std::vector<Coordinate> Map::get_enemy_spawns() const {
    return get_coord_of_blocks_with_condition([](const Block& block) {
        return block.collision == Collision::EnemySpawn;
    });
}
std::vector<Coordinate> Map::get_items_spawns() const {
    return get_coord_of_blocks_with_condition([](const Block& block) {
        return block.collision == Collision::ItemSpawn;
    });
}

std::vector<BlockOnlyTexture> Map::get_all_block_textures() const {
    return get_blocks_with_condition_and_constructor<BlockOnlyTexture>(
        [](const Block& block) { return block.has_texture(); },
        [](const Coordinate coordinate, const Block& block) {
            return BlockOnlyTexture{coordinate, block.texture};
        });
}

std::vector<BlockOnlyTexture> Map::get_all_block_textures_editor() const {
    return get_blocks_with_condition_and_constructor<BlockOnlyTexture>(
        [](const Block& block) { return block.has_texture_editor(); },
        [](const Coordinate coordinate, const Block& block) {
            return BlockOnlyTexture{coordinate, block.texture};
        });
}

IdTexture Map::get_background() const { return this->background_texture; }

Coordinate Map::get_map_size() const {
    return Coordinate{this->size_x, this->size_y};
}

void Map::add_block(const Coordinate& coordinate, const Block& block) {
    if (coordinate.x >= size_x || coordinate.y >= size_y) {
        throw OutsideMapLimitsError();
    }

    blocks[coordinate.y][coordinate.x] = block;
}

void Map::set_background(IdTexture background) {
    this->background_texture = background;
}

void Map::resizeTo(coord_unit new_size_x, coord_unit new_size_y) {
    blocks.resize(new_size_y, std::vector<::Block>(new_size_x));

    for (coord_unit y = 0; y < size_y; y++) {
        blocks[y].resize(new_size_x);
    }

    size_x = new_size_x;
    size_y = new_size_y;
}

void Map::contractToMin() {
    coord_unit new_size_x = 0;
    coord_unit new_size_y = 0;

    for (coord_unit y = 0; y < size_y; y++) {
        for (coord_unit x = 0; x < size_x; x++) {
            // Recorro todas las posiciones del mapa.
            // Si no es void, se que hasta ese punto el mapa tiene que existir.
            if (!blocks[y][x].is_void()) {
                new_size_x = std::max<coord_unit>(new_size_x, x + 1);
                new_size_y = std::max<coord_unit>(new_size_y, y + 1);
            }
        }
    }

    resizeTo(new_size_x, new_size_y);
}

void Map::expandToMax() {
    resizeTo(std::numeric_limits<coord_unit>::max(),
             std::numeric_limits<coord_unit>::max());
}