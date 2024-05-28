#ifndef ENTITY_H
#define ENTITY_H

class Entity {
private:
    int pos_x;
    int pos_y;

    int width;
    int height;
public:
    Entity(int pos_x, int pos_y, int width, int height) : pos_x(pos_x), pos_y(pos_y), width(width), height(height){};
};

#endif //ENTITY_H
