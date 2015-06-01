#ifndef OVERWORLDGAMESTATE_H
#define OVERWORLDGAMESTATE_H

#include "irrlicht.h"
#include "Artemis/Artemis.h"
#include "btBulletDynamicsCommon.h"
#include "Player.h"

#include "CharacterPhysicsComponent.h"

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

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

	artemis::Entity& entityThing(btVector3 aaa);
	artemis::Entity& makePlayer(btVector3 aaa);

	btDiscreteDynamicsWorld* dynamicsWorld;

	artemis::Entity* playerEnt;

	ChunkNode* chunkNode;
};

#endif // OVERWORLDGAMESTATE_H
