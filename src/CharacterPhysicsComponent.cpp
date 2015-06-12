#include "CharacterPhysicsComponent.h"
#include "PhysicsComponent.h"

CharacterPhysicsComponent::CharacterPhysicsComponent(
	btDynamicsWorld* const world,
	const btVector3& legStart,
	const btVector3& legEnd,
	const btScalar springStiffness,
	const btScalar springDamping,
	const btScalar footAccel,
	const btScalar footDecel,
	const btVector3& expectedGravityForce,
	const btVector3& upVector)
: artemis::Component(),
world(world),
legEnd(legEnd),
legStart(legStart),
spring(legEnd - legStart),
springStiffness(springStiffness),
springDamping(springDamping),
normalizedSpring(spring.normalized()),
feetTouchingGround(false),
footAccel(footAccel),
footDecel(footDecel),
expectedGravityForce(expectedGravityForce),
upVector(upVector),
expectedSpringCompression(expectedGravityForce / springStiffness),
targetVelocityRelativeToGround(btVector3(0, 0, 0)) {
}

CharacterPhysicsComponent::~CharacterPhysicsComponent()
{
}


CharacterPhysicsSystem::CharacterPhysicsSystem() {
	addComponentType<CharacterPhysicsComponent>();
	addComponentType<PhysicsComponent>();
}

void CharacterPhysicsSystem::initialize() {
	characterPhysicsMapper.init(*world);
	physicsMapper.init(*world);
}

void CharacterPhysicsSystem::processEntity(artemis::Entity& e) {
	PhysicsComponent* phys = physicsMapper.get(e);
	CharacterPhysicsComponent* charPhys = characterPhysicsMapper.get(e);

	// No turning?
	phys->rigidBody->setAngularFactor(0);

	// Simulate a spring under the character ("legs")
	// Since rays only register a hit if they hit a face on the "outside," we can project the rays from the inside of the character body
	btVector3 absLegStart = charPhys->legStart + phys->location;
	btVector3 absLegEnd = charPhys->legEnd + phys->location;
	btCollisionWorld::AllHitsRayResultCallback rayCallback(absLegStart, absLegEnd);
	charPhys->world->rayTest(absLegStart, absLegEnd, rayCallback);
	// Determine if the spring has actually collided with anything, since some objects do not collide with anything
	bool hasHit = false;
	btVector3 hit;
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

            	if(
					((otherGroup & phys->collidesWith) != 0) // If we collide with other
					&& ((phys->collisionGroup & otherMask) != 0)) { // and if other collides with us


					closestHitFraction = rayCallback.m_hitFractions.at(i);
					hasHit = true;
					hit = rayCallback.m_hitPointWorld.at(i);
				}
            }
		}
	}

	if(hasHit) {
		// The new length of the virtual spring
		btVector3 newLength = hit - absLegStart;

		// Calculate and apply Hooke's law with damping
		btVector3 compression = charPhys->spring - newLength; // how much the spring is "compressed"
		btVector3 energyLoss = charPhys->normalizedSpring * phys->velocity.dot(charPhys->normalizedSpring); // get the body's velocity in terms of the springs' compression
		btVector3 hooke = // Hooke's law
		-(compression * charPhys->springStiffness) // The force the spring exerts due to compression is compression multiplied by the spring's stiffness
		-(energyLoss * charPhys->springDamping); // Reduce the force due to damping
		phys->rigidBody->applyForce(hooke, btVector3(0, 0, 0));

		// Useful
		charPhys->feetTouchingGround = true;
	} else {
		charPhys->feetTouchingGround = false;
	}

	// thingy
	btVector3 impulse = charPhys->targetVelocityRelativeToGround - phys->velocity;
	impulse.setY(0);
	phys->rigidBody->applyImpulse(impulse * phys->mass, btVector3(0, 0, 0));

	// The location should be relative to the position the body should be at rest given the amount we expect the spring to compress and the length of the spring
	phys->location += -charPhys->expectedSpringCompression;
	phys->location += charPhys->spring;
}

