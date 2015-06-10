#include "ReiMath.h"

btVector3 reim::quaternionToEuler(const btQuaternion& rot) {
	btScalar x = rot.getX();
	btScalar y = rot.getY();
	btScalar z = rot.getZ();
	btScalar w = rot.getW();

	btScalar xSq = x * x;
	btScalar ySq = y * y;
	btScalar zSq = z * z;
	btScalar wSq = w * w;

	btVector3 euler;

	euler.setX(atan2f(2 * (y * z + x * w), -xSq - ySq + zSq + wSq));
	euler.setY(asinf(-2 * (x * z - y * w)));
	euler.setZ(atan2f(2 * (x * y + z * w), xSq - ySq - zSq + wSq));
	euler *= 180.0f / 3.1415926f;

	return euler;
}

irr::core::vector3df reim::bulletToIrr(const btVector3& vec) {
	return irr::core::vector3df(vec.x(), vec.y(), vec.z());
}

btVector3 reim::irrToBullet(const irr::core::vector3df& vec) {
	return btVector3(vec.X, vec.Y, vec.Z);
}


