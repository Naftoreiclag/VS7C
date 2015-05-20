#include "OverworldGameState.h"

#include "irrlicht.h"
#include "ChunkNode.h"

using namespace irr;

OverworldGameState::OverworldGameState(irr::IrrlichtDevice *irrlicht)
: GameState::GameState(irrlicht) {
	device = irrlicht;
	driver = irrlicht->getVideoDriver();
	smgr = irrlicht->getSceneManager();
}

void OverworldGameState::init() {

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	smgr->addSkyBoxSceneNode(
		driver->getTexture("example_media/irrlicht2_up.jpg"),
		driver->getTexture("example_media/irrlicht2_dn.jpg"),
		driver->getTexture("example_media/irrlicht2_lf.jpg"),
		driver->getTexture("example_media/irrlicht2_rt.jpg"),
		driver->getTexture("example_media/irrlicht2_ft.jpg"),
		driver->getTexture("example_media/irrlicht2_bk.jpg"));

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

	// Add the camera
	scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS();
	device->getCursorControl()->setVisible(false);
	cam->setPosition(core::vector3df(0,40,0));
	//smgr->addCameraSceneNode(0, core::vector3df(0,-40,0), core::vector3df(0,0,0));

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
