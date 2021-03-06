/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef REIMATH_H
#define REIMATH_H

#include <math.h>

#include "btBulletDynamicsCommon.h"
#include "irrlicht.h"

namespace reim {

    btVector3 quaternionToEuler(const btQuaternion& rot);

    btVector3 onAxis(const btVector3& input, const btVector3& axisToRemove);
    btVector3 onPlane(const btVector3& input, const btVector3& axisPerpToPlane);

    irr::core::vector3df bulletToIrr(const btVector3& vec);

    irr::f32 const degToRad = 0.01745329251f; // pi divided by 180
    irr::f32 const radToDeg = 57.2957795131f; // 180 divided by pi

    btVector3 irrToBullet(const irr::core::vector3df& vec);
}

#endif // REIMATH_H
