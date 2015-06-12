#include "ReiBullet.h"
#include "PhysicsComponent.h"

artemis::Entity* reib::entityRaycast(btCollisionWorld* world, const btVector3& start, const btVector3& end) {
	btCollisionWorld::AllHitsRayResultCallback rayCallback(start, end);
	world->rayTest(start, end, rayCallback);

	if(rayCallback.hasHit()) {
		artemis::Entity* hit = 0;

		// We cannot rely on the order of rayCallback.m_collisionObjects, so we have to compare the distances manually
		btScalar closestHitFraction(1337); // All fractions are <= 1 so this is effectively infinite
		for(int i = rayCallback.m_collisionObjects.size() - 1; i >= 0; -- i) {

			// If this result is closer than the closest valid result
			if(rayCallback.m_hitFractions.at(i) <= closestHitFraction) {
				// Get the object colliding with
				const btCollisionObject* other = rayCallback.m_collisionObjects.at(i);

				// If this result is valid
				void* userPtr = other->getUserPointer();
				if(userPtr) {
					PhysicsComponent::BulletEntityUserData* userPtrEntity = static_cast<PhysicsComponent::BulletEntityUserData*>(userPtr);
					if(userPtrEntity) {

						// Record valid result
						closestHitFraction = rayCallback.m_hitFractions.at(i);
						hit = userPtrEntity->owner;
					}
				}
			}
		}

		return hit;
	}
}
