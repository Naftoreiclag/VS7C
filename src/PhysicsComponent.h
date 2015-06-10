#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Artemis/Artemis.h"
#include "irrlicht.h"
#include "SceneNodeComponent.h"
#include "btBulletDynamicsCommon.h"

// Component that adds physics data to an entity
// Physics shaps are deleted. Use shared_ptr<btCollisionShape> to have reusable shapes.
class PhysicsComponent : public artemis::Component {
public:
	static signed short int const COLL_PLAYER = 1 << 0;
	static signed short int const COLL_ENV = 1 << 1;


private:
	bool needsAttencion; // Tells PhysicsSystem that the data is worth looking at (i.e. it changed)
	btCollisionShape* collisionShape; // For deletion
	friend class PhysicsSystem;
public:
	btQuaternion rotation;
	btVector3 location;
	btVector3 velocity;

private:
	// Utilizes the bullet calling stuff, relays information to the PhysicsComponent
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

	const btScalar mass;
	const bool isStatic; // mass == 0
    const signed short int collisionGroup;
    const signed short int collidesWith;

	PhysicsComponent(
		btDynamicsWorld* const world,
		const btScalar mass,
		btCollisionShape* collisionShape,
		const btTransform &initialLoc,
		const signed short int collisionGroup = 1, // 0000000000000001
		const signed short int collidesWith = -1); // 1111111111111111
	~PhysicsComponent();
};

// The associated system that manages all entities using PhysicsComponent
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
