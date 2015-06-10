#ifndef REIMATH_H
#define REIMATH_H

#include <math.h>

#include "btBulletDynamicsCommon.h"
#include "irrlicht.h"

namespace reim {


    btVector3 quaternionToEuler(const btQuaternion& rot);

    irr::core::vector3df bulletToIrr(const btVector3& vec);

    irr::f32 const degToRad = 0.01745329251f; // pi divided by 180
    irr::f32 const radToDeg = 57.2957795131f; // 180 divided by pi

    btVector3 irrToBullet(const irr::core::vector3df& vec);
}

#endif // REIMATH_H
