#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include <memory>
#include "entityGame.h"


class EntityFactory {
    public:
        static std::unique_ptr<Entity2> createEntity(const int& type, const int& subType);
};

#endif
