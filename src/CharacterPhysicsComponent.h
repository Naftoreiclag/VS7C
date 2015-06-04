#ifndef CHARACTERPHYSICSCOMPONENT_H
#define CHARACTERPHYSICSCOMPONENT_H

#include "irrlicht.h"
#include "Artemis/Artemis.h"
#include "btBulletDynamicsCommon.h"

class PhysicsComponent;

class CharacterPhysicsComponent : public artemis::Component {
public:
	const btVector3 legStart;
	const btVector3 legEnd;
	btDynamicsWorld* const world;
	const btVector3 spring;
	const btVector3 normalizedSpring;
	const btScalar springStiffness;
	const btScalar springDamping;

	CharacterPhysicsComponent(btDynamicsWorld* const world, const btVector3& legStart, const btVector3& legEnd, const btScalar springStiffness, const btScalar springDamping);
	~CharacterPhysicsComponent();
};

class CharacterPhysicsSystem : public artemis::EntityProcessingSystem {
private:
	artemis::ComponentMapper<CharacterPhysicsComponent> characterPhysicsMapper;
	artemis::ComponentMapper<PhysicsComponent> physicsMapper;

public:
	CharacterPhysicsSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity& e);
};

#endif // CHARACTERPHYSICSCOMPONENT_H
