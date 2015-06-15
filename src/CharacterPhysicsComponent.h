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
	btDynamicsWorld* const world;

	const btVector3 upVector; // What direction is up; is perpendicular to and defines the plane that the player walks on
	const btVector3 originOffset; // Displacement from the rigidBody's origin to where the player's origin should be
	const btVector3 expectedGravityForce; // What is the strength of gravity? Note: this does not define gravity for the character

	// Walking-related stuff
	// "Walking" is the force the player exerts on itself if isWalking to get to the velocity of targetVelocityRelativeToGround
	bool feetTouchingGround; // Whether or not the feet are in contact with some kind of ground
	bool isWalking; //
	const btRigidBody* groundBody; // Body we last stood on (default is NULL)
	btVector3 groundVelocity; // Velocity of groundBody (default is ZERO)
	btVector3 targetVelocityRelativeToGround; // Adjust this value to create "walking"; any velocity parallel to upVector is ignored (default is ZERO)


	btScalar footGrip; // Rate at which the player accel/decelerates to groundVelocity when not walking
	const btScalar footAccel; // Rate at which the player accelerates to the targetVelocity
	const btScalar minVelocityRelativeToGroundSq; // Slowest that the player can travel relative to the ground (magnitude squared); if the player's velocity is less than this amount, velocity is set to zero

	const btVector3 legStart; // Beginning of the "leg spring"; should be well within the rigidBody to prevent glitchy behavior
	const btVector3 legEnd; // Where the "leg spring" ends
	const btVector3 spring; // Vector from legStart to legEnd; calculated in constructor
	const btVector3 normalizedSpring; // Normalized form of spring; calculated in constructor
	const btScalar springStiffness; // How much force the "leg spring" will respond to given compression
	const btScalar springDamping; // The "leg spring"'s damping
	btVector3 springCompression; // Used to offset the scene node
	const btVector3 expectedSpringCompression; // How much compression is expected when the forces of gravity and the spring reach equilibrium

	CharacterPhysicsComponent(
		btDynamicsWorld* const world,
		const btVector3& originOffset,
		const btVector3& legStart,
		const btVector3& legEnd,
		const btScalar& springStiffness,
		const btScalar& springDamping,
		const btScalar& footAccel,
		const btScalar& footGrip,
		const btVector3& expectedGravityForce,
		const btVector3& upVector = btVector3(0, 1, 0),
		const btScalar minVelocityRelativeToGround = 0.3);
	~CharacterPhysicsComponent();
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
