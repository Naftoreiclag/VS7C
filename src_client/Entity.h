#ifndef ENTITY_H
#define ENTITY_H

#include "irrlicht.h"

namespace skm {

class Entity {
public:
    irr::core::vector3df loc;

    Entity();
    virtual ~Entity();
};

}

#endif // ENTITY_H
