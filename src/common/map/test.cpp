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

    assert(mapa.get_block_collision(Coordinate{1, 2}) == Collision::Air);
}

void test03PuedeObtenerTexturas() {
    Map mapa = Map::fromYaml("testmap2.yaml");

    std::vector<BlockOnlyTexture> texturas = mapa.get_all_block_textures();

    std::vector<BlockOnlyTexture> esperados;

    esperados.emplace_back(BlockOnlyTexture{Coordinate{1, 0}, "textura1"});
    esperados.emplace_back(BlockOnlyTexture{Coordinate{0, 1}, "textura2"});
    esperados.emplace_back(BlockOnlyTexture{Coordinate{1, 1}, "textura3"});
    esperados.emplace_back(BlockOnlyTexture{Coordinate{0, 2}, "textura4"});

    assert(texturas == esperados);
}

void test04lanzaExcepcionCuandoIntentoAccederOAgregarBloqueFueraDeMapa() {
    Map mapa(5, 5);

    try {
        mapa.get_block_collision(Coordinate{5, 3});
        assert(false);
    } catch (const OutsideMapLimitsError& e) {
        assert(true);
    }

    try {
        mapa.get_block_collision(Coordinate{2, 5});
        assert(false);
    } catch (const OutsideMapLimitsError& e) {
        assert(true);
    }

    try {
        mapa.get_block_collision(Coordinate{5, 5});
        assert(false);
    } catch (const OutsideMapLimitsError& e) {
        assert(true);
    }

    try {
        mapa.get_block_collision(Coordinate{10, 15});
        assert(false);
    } catch (const OutsideMapLimitsError& e) {
        assert(true);
    }

    try {
        mapa.add_block(Coordinate{10, 15}, Block{Collision::Cube, "textura1"});
        assert(false);
    } catch (const OutsideMapLimitsError& e) {
        assert(true);
    }
}

void test05siAgregoUnBloqueSoloSeAgregaEseBloque() {
    Map mapa(5, 5);

    mapa.add_block(Coordinate{2, 3}, Block{Collision::Cube, "textura1"});

    assert(mapa.get_block_collision(Coordinate{2, 3}) == Collision::Cube);
    assert(mapa.get_all_blocks_collisions().size() == 1);

    assert(mapa.get_all_block_textures().size() == 1);
    assert(mapa.get_all_block_textures()[0] ==
           (BlockOnlyTexture{Coordinate{2, 3}, "textura1"}));
}

void test06siAgregoUnBloqueNuevoEncimaDeBloqueSeSobreescribe() {
    Map mapa(5, 5);

    mapa.add_block(Coordinate{2, 3}, Block{Collision::Cube, "textura1"});
    mapa.add_block(Coordinate{2, 3},
                   Block{Collision::TriangleLowerRight, "textura2"});
    assert(mapa.get_block_collision(Coordinate{2, 3}) ==
           Collision::TriangleLowerRight);

    assert(mapa.get_all_block_textures()[0] ==
           (BlockOnlyTexture{Coordinate{2, 3}, "textura2"}));
}

int main() {
    test01GuardaBienParametrosSimples();
    test02PuedeAccederBienABloques();
    test03PuedeObtenerTexturas();
    test04lanzaExcepcionCuandoIntentoAccederOAgregarBloqueFueraDeMapa();
    test05siAgregoUnBloqueSoloSeAgregaEseBloque();
    test06siAgregoUnBloqueNuevoEncimaDeBloqueSeSobreescribe();

    std::cout << "PASARON LOS TESTS" << std::endl;
}
