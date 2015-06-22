/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "PhysicsComponent.h"

#include "ReiMath.h"

PhysicsComponent::BulletCallback::BulletCallback(const btTransform &initialLoc, PhysicsComponent* const sendTo)
: sendTo(sendTo),
initialLoc(initialLoc) {
}

void PhysicsComponent::BulletCallback::getWorldTransform(btTransform &worldTransform) const {
	worldTransform = initialLoc;
}

void PhysicsComponent::BulletCallback::setWorldTransform(const btTransform &worldTransform) {
	sendTo->rotation = worldTransform.getRotation();
	sendTo->location = worldTransform.getOrigin();
	sendTo->velocity = sendTo->rigidBody->getLinearVelocity();
	sendTo->needsAttencion = true;
}

PhysicsComponent::BulletEntityUserData::BulletEntityUserData(nres::Entity* const owner)
: owner(owner) {
}

PhysicsComponent::PhysicsComponent(
		nres::Entity* const owner,
		btDynamicsWorld* const world,
		const btScalar mass,
		btCollisionShape* collisionShape,
		const btTransform& initialLoc,
		const signed short int collisionGroup,
		const signed short int collidesWith)
: userData(new BulletEntityUserData(owner)),
collisionShape(collisionShape),
mass(mass),
isStatic(mass == 0),
world(world),
collisionGroup(collisionGroup),
collidesWith(collidesWith),
rotation(initialLoc.getRotation()),
location(initialLoc.getOrigin()),
velocity(btVector3(0, 0, 0)) {
	motionState = new BulletCallback(initialLoc, this); // OK to use "this" in constructor here
	btVector3 inertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, inertia);
	rigidBody = new btRigidBody(mass, motionState, collisionShape, inertia);
	rigidBody->setUserPointer(userData);
	world->addRigidBody(rigidBody, collisionGroup, collidesWith);
}

PhysicsComponent::~PhysicsComponent() {
	world->removeRigidBody(rigidBody);

	delete collisionShape; // Deletes the shape. If you want to have a re-usable shape, then use shared_ptr<btCollisionShape>
	delete motionState; // Not deleted by rigid body.
	delete rigidBody;
	delete userData;
}

PhysicsComponent* PhysicsComponent::clone() const {
	return 0;
}

PhysicsSystem::PhysicsSystem() {
}

const nres::ComponentID* PhysicsSystem::getComponentIDs(std::size_t& numComponentIDs) const {
	numComponentIDs = 2;
    return accessedComponents;
}

void PhysicsSystem::process(nres::Entity& e) {

	PhysicsComponent* phys = (PhysicsComponent*) e.getComponentData(compIDs::CID_PHYSICS);
	if(phys->needsAttencion) {
		SceneNodeComponent* scene = (SceneNodeComponent*) e.getComponentData(compIDs::CID_SCENE);

		scene->sceneNode->setRotation(reim::bulletToIrr(reim::quaternionToEuler(phys->rotation)));
		scene->sceneNode->setPosition(reim::bulletToIrr(phys->location));

		phys->needsAttencion = false;
	}
}


