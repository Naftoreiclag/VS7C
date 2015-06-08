#include "CharacterPhysicsComponent.h"
#include "PhysicsComponent.h"

#include <iostream>

CharacterPhysicsComponent::CharacterPhysicsComponent(
	btDynamicsWorld* const world,
	const btVector3& legStart,
	const btVector3& legEnd,
	const btScalar springStiffness,
	const btScalar springDamping,
	const btScalar footFriction,
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
footFriction(footFriction),
upVector(upVector),
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
	btCollisionWorld::ClosestRayResultCallback rayCallback(absLegStart, absLegEnd);
	charPhys->world->rayTest(absLegStart, absLegEnd, rayCallback);
	if(rayCallback.hasHit()) {
		btVector3 hit = rayCallback.m_hitPointWorld;
		//std::cout << (phys->location.y() - 0.5) + 100 << std::endl;

		// The new length of the virtual spring
		btVector3 newLength = hit - absLegStart;

		// Calculate and apply Hooke's law with damping
		btVector3 compression = charPhys->spring - newLength;
		btVector3 energyLoss = charPhys->normalizedSpring * phys->velocity.dot(charPhys->normalizedSpring);
		btVector3 hooke = -(compression * charPhys->springStiffness) - (energyLoss * charPhys->springDamping);
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
}

