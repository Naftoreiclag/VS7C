#include "OverworldGameState.h"

#include "irrlicht.h"
#include "ChunkNode.h"
#include "Chunk.h"
#include <iostream>
#include <cmath>
#include "InputManager.h"
#include "ReiMath.h"
#include "ReiBullet.h"

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

	// Load sound
	//buffer.loadFromFile("example_media/impact.wav");
	//sound.setBuffer(buffer);

	// Add the camera
	device->getCursorControl()->setVisible(false);
	yawPivot = smgr->addEmptySceneNode();
	yawPivot->setPosition(irr::core::vector3df(0, 2, 0));
	pitchPivot = smgr->addEmptySceneNode(yawPivot);
	pitchPivot->setPosition(irr::core::vector3df(1, 0, 0));
	cam = smgr->addCameraSceneNode(pitchPivot);
	cam->setPosition(core::vector3df(0, 0, -4));
	yawSpd = 0.1;
	pitchSpd = 0.1;
	maxPitch = 80;
	minPitch = -maxPitch;

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
	dynamicsWorld->addRigidBody(planeRigid, PhysicsComponent::COLL_ENV, PhysicsComponent::COLL_ENV | PhysicsComponent::COLL_PLAYER);

	playerEnt = &makePlayer(btVector3(5, 20, 5));


	SceneNodeComponent* playerSceneNode = (SceneNodeComponent*) playerEnt->getComponent<SceneNodeComponent>();
	yawPivot->setParent(playerSceneNode->sceneNode);

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

	// Update last position
	prevMouseLoc = inputMgr->getMouseLoc();
}

artemis::Entity& OverworldGameState::makePlayer(btVector3 origin) {
	// Make
	artemis::Entity& entity = entityMgr->create();

	// SceneNode
	SceneNodeComponent* sceneNode;
	{
		scene::IMeshSceneNode* node = smgr->addCubeSceneNode(1);
		node->getMaterial(0).GouraudShading = false;
		sceneNode = new SceneNodeComponent(node);
		// Do not drop resources or node, believing that Irrlicht handles that
	}

	// Physics
	PhysicsComponent* physics;
	{
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(origin);
		physics =
			new PhysicsComponent(&entity, dynamicsWorld, 1, new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)), trans,
								PhysicsComponent::COLL_PLAYER, PhysicsComponent::COLL_PLAYER | PhysicsComponent::COLL_ENV);
		physics->rigidBody->setActivationState(DISABLE_DEACTIVATION);
	}

	// Character physics
	CharacterPhysicsComponent* characterPhysics = new CharacterPhysicsComponent(
		dynamicsWorld,
		btVector3(0, 0, 0),
		btVector3(0, -1.5, 0),
		80,
		10,
		10,
		10,
		btVector3(0, -32.1522, 0)
	);

	CharacterComponent* character = new CharacterComponent();
	PlayerComponent* player = new PlayerComponent();

	// Finalize and return
	entity.addComponent(sceneNode);
	entity.addComponent(physics);
	entity.addComponent(characterPhysics);
	entity.addComponent(character);
	entity.addComponent(player);
	entity.refresh();
	return entity;
}

// NPC
artemis::Entity& OverworldGameState::entityThing(btVector3 origin) {
	// Make
	artemis::Entity& entity = entityMgr->create();

	// SceneNode
	scene::IMeshSceneNode* sceneNode = smgr->addCubeSceneNode(1);
	sceneNode->getMaterial(0).GouraudShading = false;
	entity.addComponent(new SceneNodeComponent(sceneNode));
	// Do not drop resources or node, believing that Irrlicht handles that

	// Physics
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(origin);
	PhysicsComponent* comp =
		new PhysicsComponent(&entity, dynamicsWorld, 1, new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)), trans,
							PhysicsComponent::COLL_PLAYER, PhysicsComponent::COLL_PLAYER | PhysicsComponent::COLL_ENV);
	comp->rigidBody->setActivationState(DISABLE_DEACTIVATION);
	entity.addComponent(comp);

	// Character physics
	entity.addComponent(new CharacterPhysicsComponent(dynamicsWorld, btVector3(0, 0, 0), btVector3(0, -1.5, 0), 80, 10, 10, 10, btVector3(0, -32.1522, 0)));

	// Finalize and return
	entity.refresh();
	return entity;
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
	charPhysSys->process();

	// Move the camera around

	// Looking up is negative pitch
	// Looking down is positive pitch
	// Turning left is negative yaw
	// Turning right is positive yaw

	irr::core::vector2df mouseOffset(inputMgr->getMouseLoc().X - prevMouseLoc.X, inputMgr->getMouseLoc().Y - prevMouseLoc.Y);
	mouseOffset.X *= yawSpd;
	mouseOffset.Y *= pitchSpd;
	irr::f32 newYaw = yawPivot->getRotation().Y + mouseOffset.X;
	irr::f32 newPitch = pitchPivot->getRotation().X - mouseOffset.Y;
	newPitch = newPitch > maxPitch ? maxPitch : (newPitch < minPitch ? minPitch : newPitch); // clamp pitch

	yawPivot->setRotation(irr::core::vector3df(0, newYaw, 0));
	pitchPivot->setRotation(irr::core::vector3df(newPitch, 0, 0));
	cam->setTarget(pitchPivot->getAbsolutePosition());

	// Move the player around

	PhysicsComponent* phys = (PhysicsComponent*) playerEnt->getComponent<PhysicsComponent>();
	CharacterPhysicsComponent* charPhys = (CharacterPhysicsComponent*) playerEnt->getComponent<CharacterPhysicsComponent>();

	btVector3 charRight(std::cos(-newYaw * reim::degToRad), 0, std::sin(-newYaw * reim::degToRad));
	btVector3 charForward(-charRight.z(), 0, charRight.x());
	charPhys->targetVelocityRelativeToGround.setZero();
	if(inputMgr->isKeyDown(irr::KEY_KEY_W)) {
		charPhys->targetVelocityRelativeToGround += charForward;
	}
	if(inputMgr->isKeyDown(irr::KEY_KEY_S)) {
		charPhys->targetVelocityRelativeToGround -= charForward;
	}
	if(inputMgr->isKeyDown(irr::KEY_KEY_A)) {
		charPhys->targetVelocityRelativeToGround -= charRight;
	}
	if(inputMgr->isKeyDown(irr::KEY_KEY_D)) {
		charPhys->targetVelocityRelativeToGround += charRight;
	}
	if(inputMgr->isKeyDown(irr::KEY_KEY_Q)) {
		core::line3df picker = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(inputMgr->getMouseLoc(), cam);
		// (length of picker ray is between 2999 and 3001)
		btVector3 startPt = reim::irrToBullet(picker.start);
		btVector3 endPt = reim::irrToBullet(picker.end);

		artemis::Entity* picked = reib::entityRaycast(dynamicsWorld, startPt, endPt);

		if(picked) {
            std::cout << "picky!" << std::endl;
		}

	}
	if(!charPhys->targetVelocityRelativeToGround.isZero()) {
		charPhys->targetVelocityRelativeToGround.normalize();
	}
	charPhys->targetVelocityRelativeToGround *= 5;

	// Update previous vals
	irr::s32 centerX = (irr::s32) (driver->getScreenSize().Width / 2);
	irr::s32 centerY = (irr::s32) (driver->getScreenSize().Height / 2);
	device->getCursorControl()->setPosition(centerX, centerY);
	prevMouseLoc = irr::core::position2di(centerX, centerY);
}


void OverworldGameState::render() {
	smgr->drawAll();
}
