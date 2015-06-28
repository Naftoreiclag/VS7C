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

#include "CTaskSit.h"
#include "CTaskStand.h"
#include "CharacterTaskRegistry.h"
#include "CTaskPathfind.h"

#include "RID.h"

#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

int main()
{
	LOG(INFO) << "Registering RIDs";
	RIDDatabase::addRID(0, "ERROR", "Returned when accessing an invalid RID.");

	RIDDatabase::addRID(0x44b961e67d501b91, "comp scene");
	RIDDatabase::addRID(0x8995921e44b772ba, "comp physics");
	RIDDatabase::addRID(0x921e1e349ed94eb0, "comp interaction");
	RIDDatabase::addRID(0xc8c272903f8f650b, "comp character physics");
	RIDDatabase::addRID(0x54486a4b03795e5f, "comp character identity");
	RIDDatabase::addRID(0x30689203b7f8c6f9, "comp character performer");
	RIDDatabase::addRID(0x8b9c0a77986c1e9c, "comp character body");
	RIDDatabase::addRID(0x88d8e29ee7928c59, "comp soul");
	RIDDatabase::addRID(0x7684839d77df4b64, "comp player");
	RIDDatabase::addRID(0x7371db68529979cc, "comp conversation");
//	RIDDatabase::addRID(, "");
	RIDDatabase::addRID(0x3dacd70cf7b698cc, "sitting");
	RIDDatabase::addRID(0xeb57a6cfcf7e5943, "standing");

	RIDDatabase::addRID(0x95adb8e9b5b6c2d6, "body leg left");
	RIDDatabase::addRID(0xf63fc1bddabc4907, "body foot left");
	RIDDatabase::addRID(0x1ba45f15da8817d2, "body leg right");
	RIDDatabase::addRID(0x169de19b59ea666b, "body foot right");

	CharacterTaskRegistry::addTask(new CharacterTaskSit());
	CharacterTaskRegistry::addTask(new CharacterTaskStand());
	CharacterTaskRegistry::addTask(new CTaskPathfind());

	// Get the preferred driver type
	video::E_DRIVER_TYPE driverType = driverChoiceConsole(); // video::EDT_OPENGL; //
	if(driverType == video::EDT_COUNT) { return 1; }

	// Create input manager
	InputManager inputManager;

	// Create the irrlicht device
	SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
	params.DriverType = driverType;
	params.WindowSize = core::dimension2d<u32>(1280, 720);
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
	gsmgr.attachState(initialState);

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

	LOG(INFO) << "Detaching all gamestates.";
	gsmgr.detachAll();

	LOG(INFO) << "Dropping Irrlicht device.";
	device->drop(); device = 0;

	LOG(INFO) << "Ending program.";
	return 0;
}
