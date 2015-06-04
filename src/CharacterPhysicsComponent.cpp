#include "CharacterPhysicsComponent.h"
#include "PhysicsComponent.h"

#include <iostream>;

CharacterPhysicsComponent::CharacterPhysicsComponent(btDynamicsWorld* const world, const btVector3& legStart, const btVector3& legEnd, const btScalar springStiffness, const btScalar springDamping)
: artemis::Component(),
world(world),
legEnd(legEnd),
legStart(legStart),
spring(legEnd - legStart),
springStiffness(springStiffness),
springDamping(springDamping),
normalizedSpring(spring.normalized()) {
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

	// Raycast
	btVector3 absStart = charPhys->legStart + phys->location;
	btVector3 absEnd = charPhys->legEnd + phys->location;

	btCollisionWorld::ClosestRayResultCallback rayCallback(absStart, absEnd);
	charPhys->world->rayTest(absStart, absEnd, rayCallback);


	if(rayCallback.hasHit()) {
		btVector3 hit = rayCallback.m_hitPointWorld;
		std::cout << (phys->location.y() - 0.5) + 100 << std::endl;

		// The new length of the virtual spring
		btVector3 newLength = hit - absStart;

		// Calculate and apply Hooke's law with damping
		btVector3 compression = charPhys->spring - newLength;
		btVector3 energyLoss = charPhys->normalizedSpring * phys->velocity.dot(charPhys->normalizedSpring);
		btVector3 force = -(compression * charPhys->springStiffness) - (energyLoss * charPhys->springDamping);
		phys->rigidBody->applyForce(force, btVector3(0, 0, 0));

	}
}

