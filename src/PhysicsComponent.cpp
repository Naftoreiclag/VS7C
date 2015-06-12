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

PhysicsComponent::BulletEntityUserData::BulletEntityUserData(artemis::Entity* const owner)
: owner(owner) {
}

PhysicsComponent::PhysicsComponent(
		artemis::Entity* const owner,
		btDynamicsWorld* const world,
		const btScalar mass,
		btCollisionShape* collisionShape,
		const btTransform& initialLoc,
		const signed short int collisionGroup,
		const signed short int collidesWith)
: artemis::Component(),
userData(new BulletEntityUserData(owner)),
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


