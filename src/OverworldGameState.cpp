#include "OverworldGameState.h"

#include "irrlicht.h"
#include "ChunkNode.h"



OverworldGameState::OverworldGameState(irr::IrrlichtDevice *irrlicht)
: GameState::GameState(irrlicht) {
	driver = irrlicht->getVideoDriver();
	smgr = irrlicht->getSceneManager();
}

void OverworldGameState::init() {
	// Add the camera
	smgr->addCameraSceneNode(0, core::vector3df(0,-40,0), core::vector3df(0,0,0));

	// Make the test chunk
	ChunkNode* chunkNode = new ChunkNode(smgr->getRootSceneNode(), smgr, 1337);

	// Animation
	scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(core::vector3df(0.8f, 0, 0.8f));

	if(anim) {
		chunkNode->addAnimator(anim);
		anim->drop();
		anim = 0;
	}
	chunkNode->drop();
	chunkNode = 0;
}



void OverworldGameState::cleanup() {

}

void OverworldGameState::pause()
{

}

void OverworldGameState::resume()
{
}

void OverworldGameState::render()
{

}

void OverworldGameState::update(irr::f32 tpf)
{
}
