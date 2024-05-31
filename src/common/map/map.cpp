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

        // USADO PARA CONVERTIR EN ARRAY 1D.
        //  Node nodo_bloques;

        // for (const std::vector<::Block>& fila_bloques : map.bloques) {
        //     for (const ::Block& block : fila_bloques) {
        //         nodo_bloques.push_back(block);
        //     }
        // }

        // node["blocks"] = nodo_bloques;

        return node;
    }

    static bool decode(const Node& node, ::Map& map) {
        map.map_name = node["name"].as<std::string>();

        map.size_x = node["size"]["x"].as<uint>();
        map.size_y = node["size"]["y"].as<uint>();

        map.background_texture = node["texture_background"].as<std::string>();

        // USADO PARA CONVERTIR DESDE ARRAY 1D
        //  std::vector<::Block> tira_de_bloques =
        //      node["blocks"].as<std::vector<::Block>>();

        // std::vector<std::vector<::Block>> matriz_bloques(
        //     map.size_y, std::vector<::Block>(map.size_x));

        // for (uint8_t y = 0; y < map.size_y; y++) {
        //     for (uint8_t x = 0; x < map.size_x; x++) {
        //         matriz_bloques[y][x] = tira_de_bloques[x + y * map.size_y];
        //     }
        // }

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

        block.collision = node[0].as<::BlockCollision>();

        block.texture = node[1].as<::IdTexture>();
        return true;
    }
};

template <>
struct convert<::BlockCollision> {
    static Node encode(const ::BlockCollision& blockCollision) {
        // tengo que usar uint porque sino por alguna razon
        // crea el nodo como un string de bytes, en vez de un numero
        uint number_collision = (uint)blockCollision;

        Node node(number_collision);

        return node;
    }

    static bool decode(const Node& node, ::BlockCollision& block) {
        if (!node.IsScalar()) return false;

        // Como comente antes, tengo que convertirlo a un uint,
        // porque sino hace cosas raras
        block = static_cast<BlockCollision>(node.as<uint>());

        return true;
    }
};
}  // namespace YAML

int main() {
    // // Map mapa(2, 3);

    // // mapa.bloques[0][0] = Block{BlockCollision::Air, "textura0"};
    // // mapa.bloques[0][1] = Block{BlockCollision::Cube, "textura1"};
    // // mapa.bloques[1][0] = Block{BlockCollision::Cube, "textura2"};
    // // mapa.bloques[1][1] = Block{BlockCollision::TriangleLowerRight,
    // "textura3"};
    // // mapa.bloques[2][0] = Block{BlockCollision::TriangleUpperLeft,
    // "textura4"};
    // // mapa.bloques[2][1] = Block{BlockCollision::Air, "textura5"};

    // // YAML::Node node;

    // // node["MAPA"] = mapa;

    // // // YAML::Node node;  // starts out as null

    // // // node["test"] = Block{BlockCollision::Cube, "texture1"};

    // // YAML::Emitter emitter;

    // // emitter << node;

    // std::cout << emitter.c_str() << std::endl;

    Map mapa2("testmap.yaml");

    YAML::Emitter emitter2;

    emitter2 << YAML::Node(mapa2);

    std::cout << emitter2.c_str() << std::endl;

    // YAML::Node node2;  // starts out as null

    // node2["test"] = Block{BlockCollision::Cube, "texture1"};

    // Block a = node2["test"].as<Block>();

    // std::cout << (a.collision == BlockCollision::Cube) << std::endl;
}

Map::Map() {}

Map::Map(uint8_t size_x, uint8_t size_y)
    : size_x(size_x),
      size_y(size_y),
      bloques(size_y, std::vector<::Block>(size_x)) {}

Map::Map(const char* path) {
    // TODO: COMO CONSTRUYO EL MISMO MAPA A PARTIR DE EL OBJETO YA CONSTRUIDO?
    // TAL VEZ TENGO QUE USAR UN METODO ESTATICO???

    YAML::Node nodo_mapa = YAML::LoadFile(path);

    Map mapa_temp = nodo_mapa.as<Map>();

    this->id = mapa_temp.id;
    this->size_x = mapa_temp.size_x;
    this->size_y = mapa_temp.size_y;
    this->bloques = mapa_temp.bloques;
    this->background_texture = mapa_temp.background_texture;
}
