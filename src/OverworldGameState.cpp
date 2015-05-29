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

	// Initialize bullet physics simulation
	broadphase = new btDbvtBroadphase;
	collisionConfiguration = new btDefaultCollisionConfiguration;
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -3, 0));

	// Initalize all artemis systems
	physSys = (PhysicsSystem*) systemMgr->setSystem(new PhysicsSystem());
	systemMgr->initializeAll();

	// Add the camera
	cam = smgr->addCameraSceneNode();
	cam->setPosition(core::vector3df(0, 2, -4));
	cam->setTarget(core::vector3df(0, 0, 0));

	// Make the test chunk
	ChunkMap* test = new ChunkMap(5, 5);
	chunkNode = new ChunkNode(test->getChunk(0, 0), smgr->getRootSceneNode(), smgr, 1337);

	// Phys test floor
	btStaticPlaneShape* planeShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btRigidBody* planeRigid = new btRigidBody(0, 0, planeShape);

	dynamicsWorld->addRigidBody(planeRigid);

	scene::IMesh* cube = smgr->getMesh("assets/unit_cube.dae");
	cubeNode = smgr->addMeshSceneNode(cube);
	cubeNode->setMaterialFlag(video::EMF_LIGHTING, false);

	artemis::Entity& box = entityMgr->create();
	box->addComponent(new SceneNodeComponent(cubeNode));
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(0, 10, 0));
	btCollisionShape* boxCollisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
	box->addComponent(new PhysicsComponent(dynamicsWorld, 1, boxCollisionShape, trans));
	box->refresh();

	// Cool skybox

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	smgr->addSkyBoxSceneNode(
		driver->getTexture("example_media/irrlicht2_up.jpg"),
		driver->getTexture("example_media/irrlicht2_dn.jpg"),
		driver->getTexture("example_media/irrlicht2_lf.jpg"),
		driver->getTexture("example_media/irrlicht2_rt.jpg"),
		driver->getTexture("example_media/irrlicht2_ft.jpg"),
		driver->getTexture("example_media/irrlicht2_bk.jpg"));
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
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

	physSys->process();

	cam->setPosition(cubeNode->getAbsolutePosition() + core::vector3df(0, 2, -4));
	cam->setTarget(cubeNode->getAbsolutePosition());
}


void OverworldGameState::render() {
	smgr->drawAll();
}
