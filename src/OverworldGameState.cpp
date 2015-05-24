#include "OverworldGameState.h"

#include "irrlicht.h"
#include "ChunkNode.h"
#include "Chunk.h"
#include <iostream>

using namespace irr;

OverworldGameState::OverworldGameState(irr::IrrlichtDevice *irrlicht)
: GameState::GameState(irrlicht) {
	device = irrlicht;
	driver = irrlicht->getVideoDriver();
	smgr = irrlicht->getSceneManager();

	systemMgr = entityWorld.getSystemManager();
	entityMgr = entityWorld.getEntityManager();
}

void OverworldGameState::init() {

	physSys = (PhysicsSystem*) systemMgr->setSystem(new PhysicsSystem());


	systemMgr->initializeAll();

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
	cam = smgr->addCameraSceneNode();
	cam->setPosition(core::vector3df(0, 2, -4));
	cam->setTarget(core::vector3df(0, 0, 0));

	// Make the test chunk
	ChunkMap* test = new ChunkMap(5, 5);

	chunkNode = new ChunkNode(test->getChunk(0, 0), smgr->getRootSceneNode(), smgr, 1337);

	scene::IMesh* cube = smgr->getMesh("assets/unit_cube.dae");
	scene::IMeshSceneNode* node = smgr->addMeshSceneNode(cube);
    node->setMaterialFlag(video::EMF_LIGHTING, false);

	artemis::Entity* foo = &(entityMgr->create());
	player = new Player(foo, node);
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

void OverworldGameState::update(irr::f32 tpf) {
    entityWorld.loopStart();
    entityWorld.setDelta(tpf);


	std::cout << "about to calc step" << std::endl;

	physSys->process();

	cam->setPosition(core::vector3df(player->physics->x, 2, player->physics->z -4));
	cam->setTarget(core::vector3df(player->physics->x, 0, player->physics->z));
}


void OverworldGameState::render() {
	smgr->drawAll();
}
