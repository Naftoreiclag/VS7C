#include <irrlicht.h>
#include "driverChoice.h"

#include "ChunkNode.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main() {

	// Get the preferred driver type
	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	if(driverType == video::EDT_COUNT) { return 1; }

	// Create the irrlicht device
	IrrlichtDevice *device = createDevice(driverType, core::dimension2d<u32>(640, 480), 16, false, false, false, 0);
	if(!device) { return 1; }

	// Get the driver and manager
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	// Add the camera
	smgr->addCameraSceneNode(0, core::vector3df(0,-40,0), core::vector3df(0,0,0));

	// Make the test chunk
	ChunkNode* chunkNode = new ChunkNode(smgr->getRootSceneNode(), smgr, 1337);

	// Animation
	scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(core::vector3df(0.8f, 0, 0.8f));

	if(anim) {
		chunkNode->addAnimator(anim);
		anim->drop();
		anim = 0;
	}
	chunkNode->drop();
	chunkNode = 0;

	u32 frames = 0;
	while(device->run()) {
		driver->beginScene(true, true, video::SColor(0, 100, 100, 100));

		smgr->drawAll();

		driver->endScene();

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

	device->drop();
	return 0;
}
