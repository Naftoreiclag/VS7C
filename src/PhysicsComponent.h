#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Artemis/Artemis.h"
#include "irrlicht.h"
#include "SceneNodeComponent.h"
#include "btBulletDynamicsCommon.h"

#include "ReiMath.h"

class PhysicsComponent : public artemis::Component {
public:
	bool needsAttencion;
	btQuaternion rotation;
	btVector3 location;

private:
	class BulletCallback : public btMotionState {
	protected:
		btTransform initialLoc;
		PhysicsComponent* const sendTo;
	public:
		BulletCallback(const btTransform &initialLoc, PhysicsComponent* const sendTo);

		virtual void getWorldTransform(btTransform &worldTransform) const;

		virtual void setWorldTransform(const btTransform &worldTransform);
	};

public:
	BulletCallback* motionState;
	btRigidBody* rigidBody;
	btDynamicsWorld* const world;

	PhysicsComponent(btDynamicsWorld* const world, btScalar mass, btCollisionShape* collisionShape, const btTransform &initialLoc);
	~PhysicsComponent();
};

class PhysicsSystem : public artemis::EntityProcessingSystem {
private:
	artemis::ComponentMapper<PhysicsComponent> physicsMapper;
	artemis::ComponentMapper<SceneNodeComponent> sceneNodeMapper;

public:
	PhysicsSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity& e);
};

#endif // PHYSICSCOMPONENT_H
