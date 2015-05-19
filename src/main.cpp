#include "irrlicht.h"
#include "driverChoice.h"
#include "Artemis/Artemis.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "ChunkNode.h"
#include "GameStateManager.h"
#include "OverworldGameState.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main()
{

	// Artemis test
	artemis::World world;
	artemis::SystemManager* sm = world.getSystemManager();
	artemis::EntityManager* em = world.getEntityManager();

	// Get the preferred driver type
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if(driverType == video::EDT_COUNT) { return 1; }

	// Create the irrlicht device
	IrrlichtDevice* device = createDevice(driverType, core::dimension2d<u32>(640, 480), 16, false, false, false, 0);
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
	while(device->run())
	{
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
		if(frames >= 100)
		{
			core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32) driver->getFPS();

			device->setWindowCaption(str.c_str());
			frames = 0;
		}
	}

	device->drop();
	return 0;
}
