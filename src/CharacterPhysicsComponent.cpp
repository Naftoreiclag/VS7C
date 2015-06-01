#include "CharacterPhysicsComponent.h"
#include "PhysicsComponent.h"

CharacterPhysicsComponent::CharacterPhysicsComponent()
: artemis::Component() {
	//ctor
}
CharacterPhysicsComponent::~CharacterPhysicsComponent()
{
	//dtor
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

	phys->rigidBody->setAngularFactor(0);
}

