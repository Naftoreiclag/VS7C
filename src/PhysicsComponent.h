#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Artemis/Artemis.h"
#include "irrlicht.h"
#include <iostream>

class PhysicsComponent: public artemis::Component
{
public:
	irr::f32 velX, velZ, x, z;

	PhysicsComponent();
};

class PhysicsSystem : public artemis::EntityProcessingSystem {
private:
	artemis::ComponentMapper<PhysicsComponent> physicsMapper;

public:
	PhysicsSystem() {
		addComponentType<PhysicsComponent>();

	}

	void sayHello() {
		std::cout << "hello" << std::endl;
	}
	virtual void initialize() {
		physicsMapper.init(*world);
	}
	virtual void processEntity(artemis::Entity& e) {
		std::cout << "processing" << std::endl;
		physicsMapper.get(e)->x += physicsMapper.get(e)->velX * world->getDelta();
		physicsMapper.get(e)->z += physicsMapper.get(e)->velZ * world->getDelta();

	}
	virtual bool checkProcessing() {
	return false;
	}

};

#endif // PHYSICSCOMPONENT_H
