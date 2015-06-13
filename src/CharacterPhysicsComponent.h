#ifndef CHARACTERPHYSICSCOMPONENT_H
#define CHARACTERPHYSICSCOMPONENT_H

#include "irrlicht.h"
#include "Artemis/Artemis.h"
#include "btBulletDynamicsCommon.h"

class PhysicsComponent;

/*
Must also be used with PhysicsComponent to have any effect,
since this really just modifies the PhysicsComponent's rigidBody's behavior.

Adds character-like physics behavior:
- Locks angular momentum at zero
- Floats the rigid body some distance above the ground
- Handles "walking" and "friction" by moving the character relative to the ground's velocity
*/

class CharacterPhysicsComponent : public artemis::Component {
public:
	friend class CharacterPhysicsSystem;
	btDynamicsWorld* const world; // What world this exists in

	// Walking-related stuff
	const btVector3 upVector; // What direction is up; defines the plane that the player walks on
	bool feetTouchingGround; // Whether or not the feet are in contact with some kind of ground
	const btRigidBody* groundBody; // Body we last stood on (default is NULL)
	btVector3 groundVelocity; // Velocity of groundBody (default is ZERO)
	btVector3 targetVelocityRelativeToGround; // Adjust this value to create "walking"; any velocity parallel to upVector is ignored (default is ZERO)

	btScalar footGrip; // Rate at which the player accel/decelerates to groundVelocity
	const btScalar footAccel; // Rate at which the player accelerates to the targetVelocity
	const btScalar footDecel; // Rate at which the player decelerates to the targetVelocity

	const btVector3 legStart;
	const btVector3 legEnd;
	const btVector3 spring; // Vector from legStart to legEnd
	const btVector3 normalizedSpring; // Normalized form of spring
	const btScalar springStiffness; // Measure of how much force the leg spring will respond to given compression
	const btScalar springDamping; // Measure of the spring's damping

	CharacterPhysicsComponent(
		btDynamicsWorld* const world,
		const btVector3& legStart,
		const btVector3& legEnd,
		const btScalar springStiffness,
		const btScalar springDamping,
		const btScalar footAccel,
		const btScalar footDecel,
		const btVector3& expectedGravityForce,
		const btVector3& upVector = btVector3(0, 1, 0));
	~CharacterPhysicsComponent();
private:
	const btVector3 expectedGravityForce; // What is the strength of gravity? Note: this does not define gravity for the character
	const btVector3 expectedSpringCompression; // How much compression is expected when the sum of the force of the spring and the force of gravity is zero
};

class CharacterPhysicsSystem : public artemis::EntityProcessingSystem {
private:
	artemis::ComponentMapper<CharacterPhysicsComponent> characterPhysicsMapper;
	artemis::ComponentMapper<PhysicsComponent> physicsMapper;

public:
	CharacterPhysicsSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity& e);
};

/*
class CharacterPhysicsSystemStepA : public artemis::EntityProcessingSystem {

};
*/

#endif // CHARACTERPHYSICSCOMPONENT_H
