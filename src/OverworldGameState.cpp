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

	//
	broadphase = new btDbvtBroadphase;
	collisionConfiguration = new btDefaultCollisionConfiguration;
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -1, 0));
	//physSys = (PhysicsSystem*) systemMgr->setSystem(new PhysicsSystem());

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
	cubeNode = smgr->addMeshSceneNode(cube);
	cubeNode->setMaterialFlag(video::EMF_LIGHTING, false);

	artemis::Entity* foo = &(entityMgr->create());
	player = new Player(foo, cubeNode);

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(0, 1, 0));
	motion = new SceneNodeMotionState(trans, cubeNode);
	btCollisionShape* boxCollisionShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	btRigidBody* rigidBody = new btRigidBody(1, motion, boxCollisionShape, btVector3(0, 0, 0));

	dynamicsWorld->addRigidBody(rigidBody);
}



void OverworldGameState::cleanup() {

	chunkNode->drop();
	chunkNode = 0;

	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
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

	dynamicsWorld->stepSimulation(tpf, 6);
	/*
	btTransform cubey;
	motion->getWorldTransform(cubey);
	btVector3 lock = cubey.getOrigin();

	std::cout << lock.x() << ", " << lock.y() << ", " << lock.z() << std::endl;
	cubeNode->setPosition(irr::core::vector3df(lock.x(), lock.y(), lock.z()));
	*/

	//physSys->process();

	//cam->setPosition(core::vector3df(player->physics->x, 2, player->physics->z -4));
	//cam->setTarget(core::vector3df(player->physics->x, 0, player->physics->z));

	cam->setPosition(cubeNode->getAbsolutePosition() + core::vector3df(0, 2, -4));
	cam->setTarget(cubeNode->getAbsolutePosition());
}


void OverworldGameState::render() {
	smgr->drawAll();
}
