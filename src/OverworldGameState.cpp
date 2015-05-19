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
	chunkNode = new ChunkNode(smgr->getRootSceneNode(), smgr, 1337);

	// Animation
	scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(core::vector3df(0.8f, 0, 0.8f));

	if(anim) {
		chunkNode->addAnimator(anim);
		anim->drop();
		anim = 0;
	}
}



void OverworldGameState::cleanup() {
	chunkNode->drop();
	chunkNode = 0;
}

void OverworldGameState::pause()
{

}

void OverworldGameState::resume()
{
}

void OverworldGameState::update(irr::f32 tpf)
{
}

void OverworldGameState::render() {
	smgr->drawAll();
}
