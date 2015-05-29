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
	sendTo->needsAttencion = true;
}

PhysicsComponent::PhysicsComponent(btDynamicsWorld* const world, btScalar mass, btCollisionShape* collisionShape, const btTransform &initialLoc)
: artemis::Component(),
world(world) {
	motionState = new BulletCallback(initialLoc, this); // OK to use this in constructor here
	rigidBody = new btRigidBody(mass, motionState, collisionShape);
	world->addRigidBody(rigidBody);
}

PhysicsComponent::~PhysicsComponent() {
	world->removeRigidBody(rigidBody);

	delete motionState; // might be deleted by rigid body, doubt it tho
	delete rigidBody;
}

PhysicsSystem::PhysicsSystem() {
	addComponentType<PhysicsComponent>();
	addComponentType<SceneNodeComponent>();
}

void PhysicsSystem::initialize() {
	physicsMapper.init(*world);
	sceneNodeMapper.init(*world);
}

void PhysicsSystem::processEntity(artemis::Entity& e) {

	PhysicsComponent* phys = physicsMapper.get(e);
	if(phys->needsAttencion) {
		SceneNodeComponent* scene = sceneNodeMapper.get(e);

		scene->sceneNode->setRotation(reim::bulletToIrr(reim::quaternionToEuler(phys->rotation)));
		scene->sceneNode->setPosition(reim::bulletToIrr(phys->location));

		phys->needsAttencion = false;
	}
}


