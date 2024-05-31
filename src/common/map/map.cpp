#include "map.h"

#include <fstream>
#include <iostream>

#include "yaml-cpp/yaml.h"

namespace YAML {
template <>
struct convert<::Map> {
    static Node encode(const ::Map& map) {
        Node node;

        node["name"] = map.map_name;
        node["size"]["x"] = static_cast<uint>(map.size_x);
        node["size"]["y"] = static_cast<uint>(map.size_y);

        node["texture_background"] = map.background_texture;

        node["blocks"] = map.bloques;

        return node;
    }

    static bool decode(const Node& node, ::Map& map) {
        map.map_name = node["name"].as<std::string>();

        map.size_x = node["size"]["x"].as<uint>();
        map.size_y = node["size"]["y"].as<uint>();

        map.background_texture = node["texture_background"].as<std::string>();

        map.bloques = node["blocks"].as<std::vector<std::vector<::Block>>>();

        return true;
    }
};

template <>
struct convert<::Block> {
    static Node encode(const ::Block& block) {
        Node node;
        node.push_back(block.collision);
        node.push_back(block.texture);
        return node;
    }

    static bool decode(const Node& node, ::Block& block) {
        if (!node.IsSequence() || node.size() != 2) {
            return false;
        }

        block.collision = node[0].as<::Collision>();
        block.texture = node[1].as<::IdTexture>();
        return true;
    }
};

template <>
struct convert<::Collision> {
    static Node encode(const ::Collision& blockCollision) {
        // tengo que usar uint porque sino por alguna razon
        // crea el nodo como un string de bytes, en vez de un numero
        uint number_collision = (uint)blockCollision;
        Node node(number_collision);
        return node;
    }

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

Map::Map(uint8_t size_x, uint8_t size_y)
    : size_x(size_x),
      size_y(size_y),
      bloques(size_y, std::vector<::Block>(size_x)) {}

Map Map::fromYaml(const char* path) {
    YAML::Node nodo_mapa = YAML::LoadFile(path);

    Map mapa_temp = nodo_mapa.as<Map>();

    return mapa_temp;
}

std::vector<BlockOnlyCollision> Map::get_all_blocks_collisions() const {
    std::vector<BlockOnlyCollision> block_collisions;

    for (uint y = 0; y < size_y; y++) {
        for (uint x = 0; x < size_x; x++) {
            Collision current_block_collision = bloques[y][x].collision;

            if (current_block_collision == Collision::Air) continue;

            Coordinate coord{static_cast<uint8_t>(x), static_cast<uint8_t>(y)};

            block_collisions.emplace_back(
                BlockOnlyCollision{coord, current_block_collision});
        }
    }

    return block_collisions;
}

std::string Map::get_name() const { return this->map_name; }

IdTexture Map::get_background() const { return this->background_texture; }

Coordinate Map::get_map_size() const {
    return Coordinate{this->size_x, this->size_y};
}