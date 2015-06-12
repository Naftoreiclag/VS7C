#ifndef OVERWORLDGAMESTATE_H
#define OVERWORLDGAMESTATE_H

#include "irrlicht.h"
#include "Artemis/Artemis.h"
#include "btBulletDynamicsCommon.h"

#include "CharacterComponent.h"
#include "CharacterPhysicsComponent.h"
#include "InteractionComponent.h"
#include "PhysicsComponent.h"
#include "PlayerComponent.h"
#include "SceneNodeComponent.h"
#include "SoulComponent.h"

//#include "SFML/Audio.hpp"


#include "GameState.h"

class ChunkNode;
class InputManager;

class OverworldGameState : public GameState
{
public:
	OverworldGameState(irr::IrrlichtDevice *irrlicht);

	virtual void init();
	virtual void cleanup();

	virtual void pause();
	virtual void resume();

	virtual void update(const irr::f32 tpf);
	virtual void render();
private:
	irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* smgr;
	irr::scene::ICameraSceneNode* cam;
	irr::IrrlichtDevice* device;
	InputManager* inputMgr;

	artemis::World entityWorld;
	artemis::EntityManager* entityMgr;
	artemis::SystemManager* systemMgr;

	PhysicsSystem* physSys;
	CharacterPhysicsSystem* charPhysSys;

	irr::core::position2di centerOfScreen;
	irr::f32 maxPitch;
	irr::f32 minPitch;
	irr::f32 yawSpd;
	irr::f32 pitchSpd;
	irr::scene::ISceneNode* yawPivot;
	irr::scene::ISceneNode* pitchPivot;

	//sf::SoundBuffer buffer;
	//sf::Sound sound;

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

	artemis::Entity& makeEmptyCharEnt(btVector3 aaa);

	btDiscreteDynamicsWorld* dynamicsWorld;

	artemis::Entity* playerEnt;

	irr::scene::IMeshSceneNode* chunkNode;
};

#endif // OVERWORLDGAMESTATE_H
