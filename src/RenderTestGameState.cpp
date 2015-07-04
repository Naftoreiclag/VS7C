/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "RenderTestGameState.h"

MyShaderCallback::MyShaderCallback(irr::IrrlichtDevice* device)
: device(device) {
}

MyShaderCallback::~MyShaderCallback() {
}

void MyShaderCallback::OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {
	irr::video::IVideoDriver* driver = services->getVideoDriver();

	irr::core::matrix4 invWorld = driver->getTransform(irr::video::ETS_WORLD);
	invWorld.makeInverse();
	services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);

	irr::core::matrix4 worldViewProj;
	worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);
	worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);
	worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);
	services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

	irr::core::vector3df pos = device->getSceneManager()->getActiveCamera()->getAbsolutePosition();
	services->setVertexShaderConstant("mLightPos", reinterpret_cast<irr::f32*>(&pos), 3);

	irr::video::SColorf col(0.0f, 1.0f, 1.0f, 0.0f);
	services->setVertexShaderConstant("mLightColor", reinterpret_cast<irr::f32*>(&col), 4);

	irr::core::matrix4 world = driver->getTransform(irr::video::ETS_WORLD);
	world = world.getTransposed();
	services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);

	irr::s32 TextureLayerID = 0;
	services->setPixelShaderConstant("myTexture", &TextureLayerID, 1);
}

RenderTestGameState::RenderTestGameState(irr::IrrlichtDevice *irrlicht)
: GameState::GameState(irrlicht) {
	device = irrlicht;
	driver = irrlicht->getVideoDriver();
	smgr = irrlicht->getSceneManager();
	gpu = driver->getGPUProgrammingServices();
}

RenderTestGameState::~RenderTestGameState()
{
	//dtor
}


void RenderTestGameState::init() {
	cam = smgr->getActiveCamera();
	//cam->setPosition(irr::core::vector3df(-100, 50, 100));

	// Cool skybox
	/*
	driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);
	smgr->addSkyBoxSceneNode(
		driver->getTexture("example_media/irrlicht2_up.jpg"),
		driver->getTexture("example_media/irrlicht2_dn.jpg"),
		driver->getTexture("example_media/irrlicht2_lf.jpg"),
		driver->getTexture("example_media/irrlicht2_rt.jpg"),
		driver->getTexture("example_media/irrlicht2_ft.jpg"),
		driver->getTexture("example_media/irrlicht2_bk.jpg"));
	driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, true);
	*/

	irr::video::E_MATERIAL_TYPE customMaterialType;
	MyShaderCallback* mc = new MyShaderCallback(device);

	customMaterialType = (irr::video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles(
		"assets/shaders/testShader.vert", "vertexMain", irr::video::EVST_VS_1_1,
		"assets/shaders/testShader.frag", "pixelMain", irr::video::EPST_PS_1_1,
		mc, irr::video::EMT_SOLID, 0, irr::video::EGSL_DEFAULT);

	mc->drop();
	mc = 0;

	irr::scene::ISceneNode* node = smgr->addSphereSceneNode(25);
	node->setPosition(irr::core::vector3df(0, 0, 0));
	node->setMaterialTexture(0, driver->getTexture("example_media/wall.bmp"));
	node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	node->setMaterialType(customMaterialType);
}

void RenderTestGameState::cleanup() {

}

void RenderTestGameState::pause() {

}

void RenderTestGameState::resume() {

}

void RenderTestGameState::update(const irr::f32 tpf) {

}

void RenderTestGameState::render() {
	smgr->drawAll();
}
