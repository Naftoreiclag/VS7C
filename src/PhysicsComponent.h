/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "NREntitySystem.h"
#include "irrlicht.h"
#include "SceneNodeComponent.h"
#include "btBulletDynamicsCommon.h"

#include "RID.h"

/*
Adds bullet dynamics to an entity.
Associated PhysicsSystem updates the sceneNode's orientation to match the rigid body's.
Note: physics shapes given to this component are deleted. To add resuable shapes, use shared_ptr<> instead.
*/
class PhysicsComponent : public nres::ComponentData {
public:
	static signed short int const COLL_PLAYER = 1 << 0;
	static signed short int const COLL_ENV = 1 << 1;
private:
	bool needsAttencion; // Tells PhysicsSystem that the data is worth looking at (i.e. it changed)
	friend class PhysicsSystem;
public:
	// Read-only information
	btQuaternion rotation;
	btVector3 location;
	btVector3 velocity;
public:
	// Added to bullet rigid bodies in order to associate them with the entity they belong to
	class BulletEntityUserData {
	public:
		nres::Entity* const owner;
	private:
		friend class PhysicsComponent;
		BulletEntityUserData(nres::Entity* const owner);
	};
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
		nres::Entity* const owner,
		btDynamicsWorld* const world,
		const btScalar mass,
		btCollisionShape* collisionShape,
		const btTransform &initialLoc,
		const signed short int collisionGroup = 1, // 0000000000000001
		const signed short int collidesWith = -1); // 1111111111111111
	virtual PhysicsComponent* clone() const;
	~PhysicsComponent();
private:
	btCollisionShape* collisionShape; // For deletion
	BulletEntityUserData* const userData; // Also for deletion
};

// The associated system that manages all entities using PhysicsComponent
class PhysicsSystem : public nres::System {
private:
	nres::ComponentID accessedComponents[2];
public:
	PhysicsSystem();
	virtual const nres::ComponentID* getComponentIDs(std::size_t& numComponentIDs) const;
	virtual void process(nres::Entity& e);
};

#endif // PHYSICSCOMPONENT_H
