#ifndef CHARACTERPHYSICSCOMPONENT_H
#define CHARACTERPHYSICSCOMPONENT_H

#include "irrlicht.h"
#include "Artemis/Artemis.h"

class PhysicsComponent;

class CharacterPhysicsComponent : public artemis::Component {
	public:
		irr::f32 legHeight;

		CharacterPhysicsComponent();
		~CharacterPhysicsComponent();
	protected:
	private:
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
