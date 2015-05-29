#ifndef REIMATH_H
#define REIMATH_H

#include <math.h>

#include "btBulletDynamicsCommon.h"
#include "irrlicht.h"

namespace reim {


    btVector3 quaternionToEuler(const btQuaternion &rot);

    irr::core::vector3df bulletToIrr(const btVector3 vec);
}

#endif // REIMATH_H
