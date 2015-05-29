#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Artemis/Artemis.h"
#include "irrlicht.h"
#include "SceneNodeComponent.h"
#include "btBulletDynamicsCommon.h"

#include <iostream>

class SceneNodeMotionState : public btMotionState {
protected:
	btTransform initialLoc;
	irr::scene::ISceneNode* sceneNode;
public:
	SceneNodeMotionState(const btTransform &initialLoc, irr::scene::ISceneNode* sceneNode)
	: sceneNode(sceneNode),
	initialLoc(initialLoc) {
	}

	virtual void getWorldTransform(btTransform &worldTransform) const {
		worldTransform = initialLoc;

		std::cout << "getWorld" << std::endl;
	}

	virtual void setWorldTransform(const btTransform &worldTransform) {

		std::cout << "setWorld" << std::endl;

		btQuaternion rotation = worldTransform.getRotation();
		btVector3 location = worldTransform.getOrigin();
		sceneNode->setPosition(irr::core::vector3df(location.x(), location.y(), location.z()));

		//initialLoc = worldTransform;
	}

};

class PhysicsComponent : public artemis::Component
{
public:
	btMotionState* motionState;

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
	}

};

#endif // PHYSICSCOMPONENT_H