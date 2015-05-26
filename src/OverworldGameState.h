#ifndef OVERWORLDGAMESTATE_H
#define OVERWORLDGAMESTATE_H

#include "irrlicht.h"
#include "Artemis/Artemis.h"
#include "btBulletDynamicsCommon.h"
#include "Player.h"


#include "GameState.h"

class ChunkNode;

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
	artemis::World entityWorld;
	artemis::EntityManager* entityMgr;
	artemis::SystemManager* systemMgr;

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;

	PhysicsSystem* physSys;
	artemis::Entity* foo;

	ChunkNode* chunkNode;
	Player* player;
};

#endif // OVERWORLDGAMESTATE_H
