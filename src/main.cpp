#include <irrlicht.h>
#include "driverChoice.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

// Chunk rendering
class ChunkNode : public scene::ISceneNode {

	core::aabbox3d<f32> Box;
	video::S3DVertex Vertices[4];
	video::SMaterial Material;

public:

	ChunkNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
	: scene::ISceneNode(parent, mgr, id) {
		Material.Lighting = false;

		/*

		3        2


		0        1

		*/

		Vertices[0] = video::S3DVertex(core::vector3df(  0, 0,   0), core::vector3df(0, 1, 0), video::SColor(255,   0, 255,255), core::vector2df(0, 0));
		Vertices[1] = video::S3DVertex(core::vector3df(100, 0,   0), core::vector3df(0, 1, 0), video::SColor(255, 255,   0,255), core::vector2df(1, 0));
		Vertices[2] = video::S3DVertex(core::vector3df(100, 0, 100), core::vector3df(0, 1, 0), video::SColor(255, 255, 255,  0), core::vector2df(1, 1));
		Vertices[3] = video::S3DVertex(core::vector3df(  0, 0, 100), core::vector3df(0, 1, 0), video::SColor(255,   0, 255,  0), core::vector2df(0, 1));

		Box.reset(Vertices[0].Pos);
		for (s32 i=1; i<4; ++i)
			Box.addInternalPoint(Vertices[i].Pos);
	}

	virtual void OnRegisterSceneNode() {
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}

	virtual void render() {
		u16 indices[] = {0, 1, 2, 0, 2, 3};
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setMaterial(Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 2, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
	}

	virtual const core::aabbox3d<f32>& getBoundingBox() const {
		return Box;
	}

	virtual u32 getMaterialCount() const {
		return 1;
	}

	virtual video::SMaterial& getMaterial(u32 i) {
		return Material;
	}

};

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
