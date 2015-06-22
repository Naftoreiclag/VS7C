/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterPhysicsComponent.h"
#include "PhysicsComponent.h"
#include "ReiMath.h"

/*
Component
*/

CharacterPhysicsComponent::CharacterPhysicsComponent(
	btDynamicsWorld* const world,
	const btVector3& originOffset,
	const btVector3& legStart,
	const btVector3& legEnd,
	const btScalar& springStiffness,
	const btScalar& springDamping,
	const btScalar& footAccel,
	const btScalar& footGrip,
	const btVector3& expectedGravityForce,
	const btVector3& upVector,
	const btScalar minVelocityRelativeToGround)
: world(world),
originOffset(originOffset),
legEnd(legEnd),
legStart(legStart),
spring(legEnd - legStart),
springStiffness(springStiffness),
springDamping(springDamping),
normalizedSpring(spring.normalized()),
feetTouchingGround(false),
footGrip(footGrip),
footAccel(footAccel),
minVelocityRelativeToGround(minVelocityRelativeToGround),
isWalking(false),
expectedGravityForce(expectedGravityForce),
upVector(upVector),
expectedSpringCompression(expectedGravityForce / springStiffness),
targetVelocityRelativeToGround(btVector3(0, 0, 0)),
groundVelocity(btVector3(0, 0, 0)),
springCompression(btVector3(0, 0, 0)) {
}

CharacterPhysicsComponent* CharacterPhysicsComponent::clone() const {
	return new CharacterPhysicsComponent(
										world,
										originOffset,
										legStart,
										legEnd,
										springStiffness,
										springDamping,
										footAccel,
										footGrip,
										expectedGravityForce,
										upVector,
										minVelocityRelativeToGround);
}

CharacterPhysicsComponent::~CharacterPhysicsComponent() {}

/*
System
*/

CharacterPhysicsSystem::CharacterPhysicsSystem() {}
CharacterPhysicsSystem::~CharacterPhysicsSystem() {}

const nres::ComponentID* CharacterPhysicsSystem::getComponentIDs(std::size_t& numComponentIDs) const {
	numComponentIDs = 2;
	return accessedComponents;
}

void CharacterPhysicsSystem::process(nres::Entity& e) {
	PhysicsComponent* phys = (PhysicsComponent*) e.getComponentData(compIDs::CID_PHYSICS);
	CharacterPhysicsComponent* charPhys = (CharacterPhysicsComponent*) e.getComponentData(compIDs::CID_CHARPHYSICS);

	// No turning?
	phys->rigidBody->setAngularFactor(0);
	phys->rigidBody->setAngularVelocity(btVector3(0, 0, 0));

	// Simulate a spring under the character ("legs")
	// Since rays only register a hit if they hit a face on the "outside," we can project the rays from the inside of the character body
	btVector3 absLegStart = charPhys->legStart + phys->location + charPhys->originOffset;
	btVector3 absLegEnd = charPhys->legEnd + phys->location + charPhys->originOffset;
	btCollisionWorld::AllHitsRayResultCallback rayCallback(absLegStart, absLegEnd);
	charPhys->world->rayTest(absLegStart, absLegEnd, rayCallback);

	// Determine if the spring has actually collided with anything, since some objects are marked to not collide
	bool hasHit = false;
	btVector3 hit;
	const btRigidBody* groundBody;
	if(rayCallback.hasHit()) {
		// We cannot rely on the order of rayCallback.m_collisionObjects, so we have to compare the distances manually
		btScalar closestHitFraction(1337); // All fractions are <= 1 so this is effectively infinite
		for(int i = rayCallback.m_collisionObjects.size() - 1; i >= 0; -- i) {

			// If this result is closer than the closest valid result
            if(rayCallback.m_hitFractions.at(i) <= closestHitFraction) {
            	// Get the object colliding with
            	const btCollisionObject* other = rayCallback.m_collisionObjects.at(i);

            	// If this result is valid
            	const btBroadphaseProxy* broadPhase = other->getBroadphaseHandle();
            	short signed int otherGroup = broadPhase->m_collisionFilterGroup;
            	short signed int otherMask = broadPhase->m_collisionFilterMask;

            	if(((otherGroup & phys->collidesWith) != 0) // If we collide with other
					&& ((phys->collisionGroup & otherMask) != 0)) { // and if other collides with us

					closestHitFraction = rayCallback.m_hitFractions.at(i);
					hasHit = true;
					hit = rayCallback.m_hitPointWorld.at(i);
					groundBody = static_cast<const btRigidBody*>(rayCallback.m_collisionObjects.at(i));
				}
            }
		}
	}

	// "Leg spring" hit something
	if(hasHit) {
		// Holding on to the ground
		if(groundBody) {
			charPhys->groundBody = groundBody;
			charPhys->groundVelocity = groundBody->getLinearVelocity();

			if(!charPhys->isWalking) {
				// If the character is moving too slow, just set speed to zero
				if((phys->velocity - charPhys->groundVelocity).length2() < charPhys->minVelocityRelativeToGround * charPhys->minVelocityRelativeToGround) {
					// Apply impulse to set character velocity relative to ground to zero
					btVector3 impulse = charPhys->groundVelocity - phys->velocity;
					phys->rigidBody->applyImpulse(impulse * phys->mass, btVector3(0, 0, 0));
				}

				// Gradually slow down to nothing
				else {
					btVector3 gripAccel = reim::onPlane(charPhys->groundVelocity - phys->velocity, charPhys->upVector);
					if(!gripAccel.isZero()) {
						gripAccel.normalize();
						gripAccel *= charPhys->footGrip;
						phys->rigidBody->applyForce(gripAccel * phys->mass, btVector3(0, 0, 0));
					}
				}
			}
		}

		if(charPhys->isWalking) {
			btVector3 walkAccel = reim::onPlane((charPhys->groundVelocity + charPhys->targetVelocityRelativeToGround) - phys->velocity, charPhys->upVector);
			if(!walkAccel.isZero()) {
				walkAccel.normalize();
				walkAccel *= charPhys->footAccel;
				phys->rigidBody->applyForce(walkAccel * phys->mass, btVector3(0, 0, 0));
			}
			charPhys->isWalking = false;
		}

		// The new length of the virtual spring
		btVector3 newLength = hit - absLegStart;

		// Calculate and apply Hooke's law with damping
		charPhys->springCompression = charPhys->spring - newLength; // how much the spring is "compressed"
		btVector3 energyLoss = charPhys->normalizedSpring * phys->velocity.dot(charPhys->normalizedSpring); // get the body's velocity in terms of the springs' compression
		btVector3 hooke = // Hooke's law
		-(charPhys->springCompression * charPhys->springStiffness) // The force the spring exerts due to compression is compression multiplied by the spring's stiffness
		-(energyLoss * charPhys->springDamping); // Reduce the force due to damping
		phys->rigidBody->applyForce(hooke, btVector3(0, 0, 0));

		// Useful
		charPhys->feetTouchingGround = true;
	} else {
		charPhys->feetTouchingGround = false;
	}

	// The location should be relative to:
	phys->location += charPhys->spring - charPhys->expectedSpringCompression; // ... the position the body should be at rest given the amount we expect the spring to compress
	phys->location += -charPhys->springCompression; // ... how much the spring is compressed right now
	phys->location += charPhys->originOffset; // ... that weird origin offset thing I need to do


}

