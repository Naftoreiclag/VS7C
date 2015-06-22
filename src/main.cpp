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

int main()
{
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
