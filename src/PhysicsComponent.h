#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Artemis/Artemis.h"
#include "irrlicht.h"
#include <iostream>
#include "SceneNodeComponent.h"

class PhysicsComponent : public artemis::Component
{
public:
	irr::f32 velX, velZ, x, z;

	PhysicsComponent();
};

class PhysicsSystem : public artemis::EntityProcessingSystem {
private:
	artemis::ComponentMapper<PhysicsComponent> physicsMapper;
	artemis::ComponentMapper<SceneNodeComponent> sceneNodeMapper;

public:
	PhysicsSystem() {
		addComponentType<PhysicsComponent>();
		addComponentType<SceneNodeComponent>();
	}

	virtual void initialize() {
		physicsMapper.init(*world);
		sceneNodeMapper.init(*world);
	}
	virtual void processEntity(artemis::Entity& e) {
		PhysicsComponent* phys = physicsMapper.get(e);
		SceneNodeComponent* scene = sceneNodeMapper.get(e);
		phys->x += phys->velX * world->getDelta();
		phys->z += phys->velZ * world->getDelta();

		scene->sceneNode->setPosition(irr::core::vector3df(phys->x, 0, phys->z));

	}

};

#endif // PHYSICSCOMPONENT_H
