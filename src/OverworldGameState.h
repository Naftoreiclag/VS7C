/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef OVERWORLDGAMESTATE_H
#define OVERWORLDGAMESTATE_H

#include "irrlicht.h"
#include "btBulletDynamicsCommon.h"

#include "NREntitySystem.h"

#include "CharacterIdentityComponent.h"
#include "CharacterBodyComponent.h"
#include "CharacterPhysicsComponent.h"
#include "CharacterPerformerComponent.h"
#include "InteractionComponent.h"
#include "PhysicsComponent.h"
#include "SceneNodeComponent.h"
#include "SoulComponent.h"

#include "GameState.h"
#include "InputManager.h"

#include "ReiBullet.h"

class ChunkNode;

class OverworldGameState : public GameState, public InputReceiver {
public:
	OverworldGameState(irr::IrrlichtDevice *irrlicht);

	virtual void init();
	virtual void cleanup();

	virtual void pause();
	virtual void resume();

	virtual void update(const irr::f32 tpf);
	virtual void render();

	virtual void keyPressed(irr::EKEY_CODE key);
	virtual void keyDown(irr::EKEY_CODE key);
private:
	irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* smgr;
	irr::scene::ICameraSceneNode* cam;
	irr::IrrlichtDevice* device;
	InputManager* inputMgr;

	nres::World entityWorld;

	PhysicsSystem* physSys;
	CharacterPhysicsSystem* charPhysSys;

	irr::core::position2di centerOfScreen;
	irr::f32 maxPitch;
	irr::f32 minPitch;
	irr::f32 yawSpd;
	irr::f32 pitchSpd;
	irr::scene::ISceneNode* yawPivot;
	irr::scene::ISceneNode* pitchPivot;

	reib::BulletDebugDrawer* bulletDebugDrawer;
	bool drawBulletDebug;

	//sf::SoundBuffer buffer;
	//sf::Sound sound;

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

	nres::Entity& makeEmptyCharEnt(btVector3 aaa);

	btDiscreteDynamicsWorld* dynamicsWorld;

	nres::Entity* playerEnt;

	irr::scene::IMeshSceneNode* chunkNode;
};

#endif // OVERWORLDGAMESTATE_H
