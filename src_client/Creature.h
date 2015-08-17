#ifndef CREATURE_H
#define CREATURE_H

#include "irrlicht.h"

namespace skm {

class Creature {
public:

    irr::core::vector3df loc;

    Creature();
    virtual ~Creature();
protected:
private:
};

}

#endif // CREATURE_H
