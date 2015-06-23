/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "irrlicht.h"
#include "driverChoice.h"
#include "Artemis/Artemis.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "ChunkNode.h"
#include "GameStateManager.h"
#include "OverworldGameState.h"
#include "InputManager.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#include "HierarchicalBoolean.h"
#include "HierarchicalShapeBuilder.h"
#include "HierarchicalDebugger.h"
#include <iostream>

int main()
{
	// Test
	HierarchicalShapeBuilder builder;
	builder.declareNode("footl");
	builder.declareNode("legl");
	builder.declareNode("kneel");
	builder.declareNode("footr");
	builder.declareNode("legr");
	builder.declareNode("kneer");
	builder.declareNode("pelvis");
	builder.declareNode("waistDown");

	builder.declareRelationship("waistDown", "pelvis");
	builder.declareRelationship("waistDown", "legl");
	builder.declareRelationship("waistDown", "legr");
	builder.declareRelationship("legl", "kneel");
	builder.declareRelationship("legr", "kneer");
	builder.declareRelationship("legl", "footl");
	builder.declareRelationship("legr", "footr");

	HierarchicalBooleanShape* shape = builder.makeNewBooleanShape();

	HierarchicalDebugger debugg;
	debugg.debug(shape);

	HierarchicalBoolean hbool(*shape);

	hbool.setBoolean("footl", true);
	debugg.debug(hbool);
	hbool.setBoolean("footr", true);
	debugg.debug(hbool);
	hbool.setBoolean("footl", false);
	debugg.debug(hbool);
	hbool.setWeakTrue("kneel");
	debugg.debug(hbool);

	delete shape;

	// Get the preferred driver type
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if(driverType == video::EDT_COUNT) { return 1; }

	// Create input manager
	InputManager inputManager;

	// Create the irrlicht device
	SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
	params.DriverType = driverType;
	params.WindowSize = core::dimension2d<u32>(1920, 1080);
	params.Bits = 16;
	params.Fullscreen = false;
	params.Stencilbuffer = true;
	params.Vsync = false;
	params.EventReceiver = &inputManager;
	params.AntiAlias = 2; // "Multisampling"
	IrrlichtDevice* device = createDeviceEx(params);
	if(!device) { return 1; }

	// Get the driver and manager
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	// Produce game state engine
	GameStateManager gsmgr;

	// Initial state
	GameState* initialState = new OverworldGameState(device);
	gsmgr.attachState(*initialState);
	initialState->drop();
	initialState = 0;

	// Initialize tpf calculator
	u32 then = device->getTimer()->getTime();
	u32 frames = 0;



	// Main loop
	while(device->run()) {
		// Calculate time per frame in seconds
		const u32 now = device->getTimer()->getTime();
        const f32 tpf = (f32)(now - then) / 1000.f;
        then = now;

		// Update game state manager
		gsmgr.update(tpf);

		// Clear buffers before rendering
		driver->beginScene(true, true, video::SColor(0, 255, 255, 255));

		// Draw stuff
		gsmgr.render();

		// Flush
		driver->endScene();

		// Debug counter
		++ frames;
		if(frames >= 100) {
			core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32) driver->getFPS();

			device->setWindowCaption(str.c_str());
			frames = 0;
		}
	}

	device->drop(); device = 0;
	return 0;
}
