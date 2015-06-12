#ifndef REIBULLET_H
#define REIBULLET_H

#include "Artemis/Artemis.h"
#include "btBulletDynamicsCommon.h"

namespace reib {
	artemis::Entity* entityRaycast(btCollisionWorld* world, const btVector3& start, const btVector3& end);
};

#endif // REIBULLET_H
