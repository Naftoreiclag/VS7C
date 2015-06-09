#include "OverworldGameState.h"

#include "irrlicht.h"
#include "ChunkNode.h"
#include "Chunk.h"
#include <iostream>
#include "InputManager.h"

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
	charPhysSys = (CharacterPhysicsSystem*) systemMgr->setSystem(new CharacterPhysicsSystem());
	systemMgr->initializeAll();


	// Add the camera
	cam = smgr->addCameraSceneNode();
	cam->setPosition(core::vector3df(0, 2, -4));
	cam->setTarget(core::vector3df(0, 0, 0));


	irr::scene::ISceneNode* empt = smgr->addEmptySceneNode();
	irr::scene::ILightSceneNode* dlight = smgr->addLightSceneNode(empt, irr::core::vector3df(0, 0, 1), irr::video::SColor(1, 1, 1, 1), 10000.0f);
	irr::video::SLight light;
	light.Type = irr::video::ELT_DIRECTIONAL;
	light.Direction = irr::core::vector3df(0, -100, 0);
	light.AmbientColor = irr::video::SColorf(0.3f, 0.3f, 0.6f, 1);
	light.SpecularColor= irr::video::SColorf(1.0f, 1.0f, 1.0f, 1);
	light.DiffuseColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1);
	light.CastShadows = false;
	dlight->setLightData(light);

	// ???
	smgr->setShadowColor(video::SColor(149, 0, 0, 0));


	// Make the test chunk
	//ChunkMap* test = new ChunkMap(5, 5);
	//chunkNode = new ChunkNode(test->getChunk(0, 0), smgr->getRootSceneNode(), smgr, 1337);
	//

	chunkNode = smgr->addCubeSceneNode(100);
	chunkNode->getMaterial(0).setTexture(0, driver->getTexture("assets/grass.png"));
	chunkNode->setPosition(irr::core::vector3df(0, -50, 0));
	chunkNode->addShadowVolumeSceneNode();

	// Phys test floor
	btStaticPlaneShape* planeShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btRigidBody* planeRigid = new btRigidBody(0, 0, planeShape);
	dynamicsWorld->addRigidBody(planeRigid);

	artemis::Entity& entity = entityThing(btVector3(3, 3, 3));
	//SceneNodeComponent* snc = (SceneNodeComponent*) entity.getComponent<SceneNodeComponent>();
	//dlight->setParent(snc->sceneNode);
	entityThing(btVector3(3.7, 6, 3.7));
	entityThing(btVector3(4.1, 10, 4.1));
	entityThing(btVector3(6, 6, 6));
	entityThing(btVector3(8, 6, 6));
	entityThing(btVector3(6, 6, 8));


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
	scene::IMeshSceneNode* sceneNode = smgr->addCubeSceneNode(1);
	sceneNode->getMaterial(0).GouraudShading = false;
	sceneNode->addShadowVolumeSceneNode();
	entity.addComponent(new SceneNodeComponent(sceneNode));
	// Do not drop resources or node, believing that Irrlicht handles that

	// Physics
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(origin);
	PhysicsComponent* comp = new PhysicsComponent(dynamicsWorld, 1, new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)), trans, 1 << 0, 0);
	comp->rigidBody->setActivationState(DISABLE_DEACTIVATION);
	entity.addComponent(comp);
	entity.addComponent(new CharacterPhysicsComponent(dynamicsWorld, btVector3(0, 0, 0), btVector3(0, -1.5, 0), 80, 10, 10, 10, btVector3(0, -32.1522, 0)));

	// Finalize and return
	entity.refresh();
	return entity;
}

artemis::Entity& OverworldGameState::entityThing(btVector3 origin) {
	// Box entity
	artemis::Entity& box = entityMgr->create();

	// SceneNode
	//scene::IMesh* cube = smgr->getMesh("assets/unit_sphere.dae");
	scene::IMeshSceneNode* sceneNode = smgr->addSphereSceneNode(0.5f);
	sceneNode->getMaterial(0).GouraudShading = true;
	sceneNode->addShadowVolumeSceneNode();
	box.addComponent(new SceneNodeComponent(sceneNode));
	// Do not drop resources or node, believing that Irrlicht handles that

	// Physics
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(origin);
	box.addComponent(new PhysicsComponent(dynamicsWorld, 8, new btSphereShape(0.5), trans));

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

	PhysicsComponent* phys = (PhysicsComponent*) playerEnt->getComponent<PhysicsComponent>();
	CharacterPhysicsComponent* charPhys = (CharacterPhysicsComponent*) playerEnt->getComponent<CharacterPhysicsComponent>();

	charPhys->targetVelocityRelativeToGround.setZero();
	if(inputMgr->isKeyDown(irr::KEY_KEY_W)) {
		charPhys->targetVelocityRelativeToGround.setZ(1);
	}
	if(inputMgr->isKeyDown(irr::KEY_KEY_S)) {
		charPhys->targetVelocityRelativeToGround.setZ(-1);
	}
	if(inputMgr->isKeyDown(irr::KEY_KEY_A)) {
		charPhys->targetVelocityRelativeToGround.setX(-1);
	}
	if(inputMgr->isKeyDown(irr::KEY_KEY_D)) {
		charPhys->targetVelocityRelativeToGround.setX(1);
	}
	if(!charPhys->targetVelocityRelativeToGround.isZero()) {
		charPhys->targetVelocityRelativeToGround.normalize();
	}
	charPhys->targetVelocityRelativeToGround *= 5;

	dynamicsWorld->stepSimulation(tpf, 6);
	physSys->process();
	charPhysSys->process();

/*
	std::cout << phys->location.x() << ",\t";
	std::cout << phys->location.y() << ",\t";
	std::cout << phys->location.z() << std::endl;
	*/

	SceneNodeComponent* comp = (SceneNodeComponent*) playerEnt->getComponent<SceneNodeComponent>();

	// Camera stuff


	cam->setPosition(comp->sceneNode->getAbsolutePosition() + core::vector3df(0, 2, -4));
	cam->setTarget(comp->sceneNode->getAbsolutePosition());
}


void OverworldGameState::render() {
	smgr->drawAll();
}
