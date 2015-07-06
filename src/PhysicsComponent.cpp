/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "PhysicsComponent.h"

#include "ReiMath.h"
#include <iostream>

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
	std::cout << "phys remove rigid body" << std::endl;
	std::cout << rigidBody << std::endl;
	world->removeRigidBody(rigidBody);

	std::cout << "phys delete shape" << std::endl;
	delete collisionShape; // Deletes the shape. If you want to have a re-usable shape, then use shared_ptr<btCollisionShape>
	std::cout << "phys motion" << std::endl;
	delete motionState; // Not deleted by rigid body.
	std::cout << "phys rigid" << std::endl;
	delete rigidBody;
	std::cout << "phys userd" << std::endl;
	delete userData;
}

PhysicsComponent* PhysicsComponent::clone() const {
	return 0;
}

PhysicsSystem::PhysicsSystem() {
	accessedComponents[0] = RID("comp physics");
	accessedComponents[1] = RID("comp scene");
}

const nres::ComponentID* PhysicsSystem::getComponentIDs(std::size_t& numComponentIDs) const {
	numComponentIDs = 2;
    return accessedComponents;
}

void PhysicsSystem::process(nres::Entity& e) {

	PhysicsComponent* phys = (PhysicsComponent*) e.getComponentData(RID("comp physics"));
	if(phys->needsAttencion) {
		SceneNodeComponent* scene = (SceneNodeComponent*) e.getComponentData(RID("comp scene"));

		scene->sceneNode->setRotation(reim::bulletToIrr(reim::quaternionToEuler(phys->rotation)));
		scene->sceneNode->setPosition(reim::bulletToIrr(phys->location));

		phys->needsAttencion = false;
	}
}

nres::ComponentData* PhysicsSerializer::read(const Json::Value& data) {
	//PhysicsComponent* comp = new PhysicsComponent();



	return 0;
}

Json::Value PhysicsSerializer::write(const nres::ComponentData* compData) {
	const PhysicsComponent* comp = static_cast<const PhysicsComponent*>(compData);
	Json::Value data;

	return data;
}



