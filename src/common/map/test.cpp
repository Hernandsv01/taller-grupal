#include <cassert>
#include <iostream>

#include "map.h"

void test01GuardaBienParametrosSimples() {
    Map mapa = Map::fromYaml("testmap.yaml");

    assert(mapa.get_name() == "Mapa Test");
    assert(mapa.get_background() == "texture4");
    assert(mapa.get_map_size() == (Coordinate{5, 4}));
}

void test02PuedeAccederBienABloques() {
    Map mapa = Map::fromYaml("testmap2.yaml");

    std::vector<BlockOnlyCollision> colisiones =
        mapa.get_all_blocks_collisions();

    std::vector<BlockOnlyCollision> esperados;

    esperados.emplace_back(
        BlockOnlyCollision{Coordinate{1, 0}, Collision::Cube});
    esperados.emplace_back(
        BlockOnlyCollision{Coordinate{0, 1}, Collision::Cube});
    esperados.emplace_back(
        BlockOnlyCollision{Coordinate{1, 1}, Collision::TriangleLowerRight});
    esperados.emplace_back(
        BlockOnlyCollision{Coordinate{0, 2}, Collision::TriangleUpperLeft});

    assert(colisiones == esperados);
}

int main() {
    test01GuardaBienParametrosSimples();

    test02PuedeAccederBienABloques();
    std::cout << "PASARON LOS TESTS" << std::endl;
}
