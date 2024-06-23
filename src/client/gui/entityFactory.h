#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include <memory>

#include "entityGame.h"

class EntityFactory {
   private:
    static const std::vector<std::string> subtype_chart;

   public:
    static std::shared_ptr<Entity2> createEntity(const int& type,
                                                 const int& subType);
};

#endif
