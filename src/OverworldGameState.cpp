/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "OverworldGameState.h"

#include "irrlicht.h"
#include "ChunkNode.h"
#include "Chunk.h"
#include <iostream>
#include <cmath>
#include "InputManager.h"
#include "ReiMath.h"
#include "ReiBullet.h"

#include "CTConditionSitting.h"
#include "CTConditionLocation.h"

#include "easylogging++.h"

using namespace irr;

OverworldGameState::OverworldGameState(irr::IrrlichtDevice *irrlicht)
: GameState::GameState(irrlicht) {
	device = irrlicht;
	driver = irrlicht->getVideoDriver();
	smgr = irrlicht->getSceneManager();
	inputMgr = (InputManager*) irrlicht->getEventReceiver();
}

void OverworldGameState::init() {

	// Initialize bullet physics simulation
	broadphase = new btDbvtBroadphase;
	collisionConfiguration = new btDefaultCollisionConfiguration;
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -32.1522, 0)); // Earth gravity
	bulletDebugDrawer = new reib::BulletDebugDrawer(device);
	drawBulletDebug = true;
	dynamicsWorld->setDebugDrawer(bulletDebugDrawer);

	// Initialize entity systems
	entityWorld = new nres::World();
	physSys = new PhysicsSystem();
	charPhysSys = new CharacterPhysicsSystem();
	charPerfSys = new CharacterPerformerSystem();

	// Calc center of screen
	irr::s32 centerX = (irr::s32) (driver->getScreenSize().Width / 2);
	irr::s32 centerY = (irr::s32) (driver->getScreenSize().Height / 2);
	centerOfScreen = irr::core::position2di(centerX, centerY);

	// Add the camera
	device->getCursorControl()->setVisible(false);
	yawPivot = smgr->addEmptySceneNode();
	yawPivot->setPosition(irr::core::vector3df(0, 3, 0));
	pitchPivot = smgr->addEmptySceneNode(yawPivot);
	pitchPivot->setPosition(irr::core::vector3df(2, 0, 0));
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

	playerEnt = &makeEmptyCharEnt(btVector3(5, 21, 5));
	playerEnt->finalize();

	nres::Entity& sammy = makeEmptyCharEnt(btVector3(15, 20, 5));
    sammy.addComponent(RID("comp soul"), new SoulComponent());
    sammy.finalize();


	SceneNodeComponent* playerSceneNode = (SceneNodeComponent*) playerEnt->getComponentData(RID("comp scene"));
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
}

nres::Entity& OverworldGameState::makeEmptyCharEnt(btVector3 origin) {
	// Make
	nres::Entity& entity = entityWorld->newEntity();

	CharacterIdentityComponent* characterIdentity = new CharacterIdentityComponent();
	CharacterBodyComponent* characterBody = new CharacterBodyComponent();
	CharacterPerformerComponent* characterPerformer = new CharacterPerformerComponent();

	// SceneNode
	SceneNodeComponent* sceneNode;
	{
		scene::ISceneNode* node = smgr->addEmptySceneNode();
		scene::IMeshSceneNode* cube = smgr->addSphereSceneNode(1, 16, node);
		cube->getMaterial(0).GouraudShading = false;
		cube->setPosition(irr::core::vector3df(0, 0, 0));
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
			new PhysicsComponent(&entity, dynamicsWorld, 1, new btCapsuleShape(1.0f, 1.5f), trans,
								PhysicsComponent::COLL_PLAYER, PhysicsComponent::COLL_PLAYER | PhysicsComponent::COLL_ENV);
		physics->rigidBody->setActivationState(DISABLE_DEACTIVATION);
	}

	// Character physics
	CharacterPhysicsComponent* characterPhysics = new CharacterPhysicsComponent(
		dynamicsWorld,
		btVector3(0, -3.5f/2.0f, 0),
		btVector3(0, 0.5, 0),
		btVector3(0, -1.0, 0),
		80,
		10,
		30, // foot accel
		20,
		btVector3(0, -32.1522, 0)
	);

	entity.addComponent(RID("comp scene"), sceneNode);
	entity.addComponent(RID("comp physics"), physics);
	entity.addComponent(RID("comp character physics"), characterPhysics);
	entity.addComponent(RID("comp character body"), characterBody);
	entity.addComponent(RID("comp character identity"), characterIdentity);
	entity.addComponent(RID("comp character performer"), characterPerformer);
	return entity;
}

void OverworldGameState::cleanup() {
}

OverworldGameState::~OverworldGameState() {
	chunkNode->drop();
	chunkNode = 0;

	// delete world before deleting bullet physics
	delete physSys;
	delete charPerfSys;
	delete charPhysSys;
	LOG(INFO) << "Entity systems deleted.";

	delete entityWorld;
	LOG(INFO) << "Entity world deleted.";

	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
	LOG(INFO) << "Bullet physics simulation deleted.";

	delete bulletDebugDrawer;
	LOG(INFO) << "Bullet physics debug drawer.";
}

void OverworldGameState::pause()
{

}

void OverworldGameState::resume()
{
}

void OverworldGameState::update(irr::f32 tpf) {
	dynamicsWorld->stepSimulation(tpf, 6); // Resolve all collisions, velocities, and forces
	entityWorld->process(*charPhysSys); // Do special physics
	entityWorld->process(*physSys); // Update scenenodes
	charPerfSys->setTpf(tpf);
	entityWorld->process(*charPerfSys);

	// Move the camera around

	// Looking up is negative pitch
	// Looking down is positive pitch
	// Turning left is negative yaw
	// Turning right is positive yaw

	irr::core::vector2df mouseOffset(inputMgr->getMouseLoc().X - centerOfScreen.X, inputMgr->getMouseLoc().Y - centerOfScreen.Y);
	mouseOffset.X *= yawSpd;
	mouseOffset.Y *= pitchSpd;
	irr::f32 newYaw = yawPivot->getRotation().Y + mouseOffset.X;
	irr::f32 newPitch = pitchPivot->getRotation().X - mouseOffset.Y;
	newPitch = newPitch > maxPitch ? maxPitch : (newPitch < minPitch ? minPitch : newPitch); // clamp pitch

	yawPivot->setRotation(irr::core::vector3df(0, newYaw, 0));
	pitchPivot->setRotation(irr::core::vector3df(newPitch, 0, 0));
	cam->setTarget(pitchPivot->getAbsolutePosition());

	// Move the player around


	PhysicsComponent* phys = (PhysicsComponent*) playerEnt->getComponentData(RID("comp physics"));
	CharacterPhysicsComponent* charPhys = (CharacterPhysicsComponent*) playerEnt->getComponentData(RID("comp character physics"));

	inputMgr->notifyMe(this);

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
	if(!charPhys->targetVelocityRelativeToGround.isZero()) {
		charPhys->targetVelocityRelativeToGround.normalize();
		charPhys->isWalking = true;
	}
	charPhys->targetVelocityRelativeToGround *= 5;

	device->getCursorControl()->setPosition(centerOfScreen);
}

void OverworldGameState::keyPressed(irr::EKEY_CODE key) {

	if(key == irr::KEY_KEY_Q) {
		makeEmptyCharEnt(btVector3(15, 20, 5)).finalize();

	}
	if(key == irr::KEY_KEY_K) {
		CharacterPerformerComponent* charPerf = (CharacterPerformerComponent*) playerEnt->getComponentData(RID("comp character performer"));

		CharacterTaskCondition* aaa = new CTConditionSitting(true);
		std::cout << "set current objective" << std::endl;
		charPerf->currentObjective.conditionToFulfill = aaa;

	}
	if(key == irr::KEY_KEY_B) {
		CharacterPerformerComponent* charPerf = (CharacterPerformerComponent*) playerEnt->getComponentData(RID("comp character performer"));

		CharacterTaskCondition* aaa = new CTConditionSitting(false);
		std::cout << "set current objective" << std::endl;
		charPerf->currentObjective.conditionToFulfill = aaa;

	}
	if(key == irr::KEY_KEY_R) {
		CharacterPerformerComponent* charPerf = (CharacterPerformerComponent*) playerEnt->getComponentData(RID("comp character performer"));

		CharacterTaskCondition* aaa = new CTConditionLocation(2, btVector3(0, 0, 0));
		std::cout << "set current objective" << std::endl;
		charPerf->currentObjective.conditionToFulfill = aaa;

	}
	if(key == irr::KEY_KEY_F) {
		core::line3df picker = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(inputMgr->getMouseLoc(), cam);
		// (length of picker ray is between 2999 and 3001)
		btVector3 startPt = reim::irrToBullet(picker.start);
		btVector3 endPt = reim::irrToBullet(picker.end);

		nres::Entity* picked = reib::entityRaycast(dynamicsWorld, startPt, endPt);

		if(picked) {

		nres::ComponentData* data = picked->getComponentData(RID("comp interaction"));
		if(data) {
			InteractionComponent* interData = (InteractionComponent*) data;
		}

		}


	}
	std::cout << key << std::endl;
}

void OverworldGameState::keyDown(irr::EKEY_CODE key) {
	if(key == irr::KEY_SPACE) {
		PhysicsComponent* phys = (PhysicsComponent*) playerEnt->getComponentData(RID("comp physics"));
		phys->rigidBody->applyForce(btVector3(0, 40, 0), btVector3(0, 0, 0));
	}
}

void OverworldGameState::render() {
	smgr->drawAll();

	// crosshair
	driver->draw2DRectangle(
		irr::video::SColor(255, 255, 255, 255),
		irr::core::rect<s32>(
			centerOfScreen.X - 2, centerOfScreen.Y - 2,
			centerOfScreen.X + 2, centerOfScreen.Y + 2));

	// Something else
	if(drawBulletDebug) {
		driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
		driver->setMaterial(bulletDebugDrawer->material);
		dynamicsWorld->debugDrawWorld();
	}
}
