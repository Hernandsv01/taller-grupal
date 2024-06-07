#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

#define xMapDim 768
#define yMapDim 1024

#define backWidth 192

#define partWidth 32
#define partHeight 64

#define wallDim 256

enum terrainParts { floorPos=0, underPos=1, backPos=2, wallPos=3};

struct Map {
    int xDimension = 768;
    int yDimension = 1024;
};


#endif
