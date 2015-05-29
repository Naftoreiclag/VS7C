#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Artemis/Artemis.h"
#include "irrlicht.h"
#include "SceneNodeComponent.h"
#include "btBulletDynamicsCommon.h"

#include "ReiMath.h"

/*
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
	}

	virtual void setWorldTransform(const btTransform &worldTransform) {
		btQuaternion rotation = worldTransform.getRotation();
		sceneNode->setRotation(reim::bulletToIrr(reim::quaternionToEuler(rotation)));

		btVector3 location = worldTransform.getOrigin();
		sceneNode->setPosition(irr::core::vector3df(location.x(), location.y(), location.z()));
	}

};
*/

class PhysicsComponent : public artemis::Component
{
public:
	bool needsAttencion;
	btQuaternion rotation;
	btVector3 location;
private:
	class PhysicsComponentMotionState : public btMotionState {
	protected:
		btTransform initialLoc;
		PhysicsComponent* const sendTo;
	public:
		PhysicsComponentMotionState(const btTransform &initialLoc, PhysicsComponent* const sendTo)
		: sendTo(sendTo),
		initialLoc(initialLoc) {
		}

		virtual void getWorldTransform(btTransform &worldTransform) const {
			worldTransform = initialLoc;
		}

		virtual void setWorldTransform(const btTransform &worldTransform) {
			sendTo->rotation = worldTransform.getRotation();
			sendTo->location = worldTransform.getOrigin();
			sendTo->needsAttencion = true;
		}
	};
public:
	PhysicsComponentMotionState* motionState;
	btRigidBody* rigidBody;
	btDynamicsWorld* const world;

	PhysicsComponent(btDynamicsWorld* const world, btScalar mass, btCollisionShape* collisionShape, const btTransform &initialLoc)
	: artemis::Component(),
	world(world) {
		motionState = new PhysicsComponentMotionState(initialLoc, this);

		rigidBody = new btRigidBody(mass, motionState, collisionShape);
		world->addRigidBody(rigidBody);
	}

	~PhysicsComponent() {
		world->removeRigidBody(rigidBody);

		delete motionState; // might be deleted by rigid body, doubt it tho
		delete rigidBody;
	}
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
		//if(phys->needsAttencion)
		SceneNodeComponent* scene = sceneNodeMapper.get(e);

		scene->sceneNode->setRotation(reim::bulletToIrr(reim::quaternionToEuler(phys->rotation)));
		scene->sceneNode->setPosition(reim::bulletToIrr(phys->location));
	}

};

#endif // PHYSICSCOMPONENT_H
