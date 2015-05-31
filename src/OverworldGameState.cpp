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
	inputMgr = (InputManager*) irrlicht->getEventReceiver();

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
	dynamicsWorld->setGravity(btVector3(0, -32.1522, 0)); // Earth gravity

	// Initalize all artemis systems
	physSys = (PhysicsSystem*) systemMgr->setSystem(new PhysicsSystem());
	systemMgr->initializeAll();


	// Add the camera
	cam = smgr->addCameraSceneNode();
	cam->setPosition(core::vector3df(0, 2, -4));
	cam->setTarget(core::vector3df(0, 0, 0));

	// Light?
	irr::scene::ILightSceneNode* light = smgr->addLightSceneNode();
	light->setLightType(irr::video::ELT_DIRECTIONAL);
	light->setPosition(irr::core::vector3df(0, 1, 0));
	smgr->setAmbientLight(irr::video::SColor(1, 1, 1, 0));

	// Make the test chunk
	ChunkMap* test = new ChunkMap(5, 5);
	chunkNode = new ChunkNode(test->getChunk(0, 0), smgr->getRootSceneNode(), smgr, 1337);

	// Phys test floor
	btStaticPlaneShape* planeShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btRigidBody* planeRigid = new btRigidBody(0, 0, planeShape);
	dynamicsWorld->addRigidBody(planeRigid);

	entityThing(btVector3(3, 3, 3));
	entityThing(btVector3(3.7, 6, 3.7));
	entityThing(btVector3(4.1, 10, 4.1));
	entityThing(btVector3(6, 6, 6));


	playerEnt = &makePlayer(btVector3(5, 5, 5));

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

artemis::Entity& OverworldGameState::makePlayer(btVector3 origin) {
	// Make
	artemis::Entity& entity = entityMgr->create();

	// SceneNode
	scene::IMesh* cube = smgr->getMesh("assets/unit_cube.dae");
	entity.addComponent(new SceneNodeComponent(smgr->addMeshSceneNode(cube)));
	// Do not drop resources or node, believing that Irrlicht handles that

	// Physics
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(origin);
	entity.addComponent(new PhysicsComponent(dynamicsWorld, 8, new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)), trans));

	// Finalize and return
	entity.refresh();
	return entity;
}

artemis::Entity& OverworldGameState::entityThing(btVector3 origin) {
	// Box entity
	artemis::Entity& box = entityMgr->create();

	// SceneNode
	scene::IMesh* cube = smgr->getMesh("assets/unit_cube.dae");
	box.addComponent(new SceneNodeComponent(smgr->addMeshSceneNode(cube)));
	// Do not drop resources or node, believing that Irrlicht handles that

	// Physics
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(origin);
	box.addComponent(new PhysicsComponent(dynamicsWorld, 8, new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)), trans));

	// Finalize box entity
	box.refresh();

	return box;

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

	SceneNodeComponent* comp = (SceneNodeComponent*) playerEnt->getComponent<SceneNodeComponent>();

	cam->setPosition(comp->sceneNode->getAbsolutePosition() + core::vector3df(0, 2, -4));
	cam->setTarget(comp->sceneNode->getAbsolutePosition());
}


void OverworldGameState::render() {
	smgr->drawAll();
}
