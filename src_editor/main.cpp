/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "irrlicht.h"
#include "driverChoice.h"
#include "json/json.h"
#include "btBulletCollisionCommon.h"
#include "ReiBullet.h"

irr::video::IVideoDriver* driver;
irr::scene::ISceneManager* smgr;
irr::gui::IGUIEnvironment* gui;

btBroadphaseInterface* broadphase;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;

btCollisionWorld* bulletWorld;
reib::BulletDebugDrawer* bulletDebugDrawer;


enum {

	GUI_ROOT_WINDOW = 0x10000,

	GUI_FILE_NEW,
	GUI_FILE_OPEN,
	GUI_FILE_SAVE,
	GUI_FILE_SAVE_AS,

	GUI_DIALOG_RESOURCES,
	GUI_DIALOG_COMPONENTS,
	GUI_DIALOG_MODEL,
	GUI_DIALOG_PHYSICS,
	GUI_DIALOG_ANIMATION,

	GUI_TREE_RESOURCES,

    GUI_BUTTON_RESOURCES,
    GUI_BUTTON_COMPONENTS,
    GUI_BUTTON_MODEL,
    GUI_BUTTON_PHYSICS,
    GUI_BUTTON_ANIMATION,

	// Y-axis aligned where applicable
	PHYS_EMPTY,
    PHYS_SPHERE,
    PHYS_BOX,
    PHYS_CYLINDER,
    PHYS_CAPSULE,
    PHYS_CONE,
    PHYS_MULTI_SPHERE,
    PHYS_TRIANGLE_MESH,
};

struct PhysicsShape {
	bool modified = false;

	irr::s32 type = PHYS_EMPTY;

	btScalar radius = 0; // Sphere, Capsule, Cone
	btVector3 dimensions = btVector3(0, 0, 0); // Box, Cylinder
	btScalar height = 0; // Capsule, Cone

	std::vector<std::pair<btVector3, btScalar>> locRadi; // MULTI_SPHERE
	btTriangleMesh* triangles = 0; // TRIANGLE_MESH
};

struct Project {
	std::string dir;

	Json::Value jsonFile;
	PhysicsShape physicsShape;
};

Project* currProj;

irr::scene::ISceneNode* rootNode;

inline irr::core::rect<irr::s32> GuiBox(irr::s32 x, irr::s32 y, irr::s32 width, irr::s32 height) {
	return irr::core::rect<irr::s32>(x, y, x + width, y + height);
}
inline irr::core::rect<irr::s32> GuiRect(irr::s32 x1, irr::s32 y1, irr::s32 x2, irr::s32 y2) {
	return irr::core::rect<irr::s32>(x1, y1, x2, y2);
}

btCollisionShape* newShape(PhysicsShape shape) {
    switch(shape.type) {
		case PHYS_SPHERE: {
			return new btSphereShape(shape.radius);
		}
		case PHYS_BOX: {
			std::cout << "Generated shape from box." << std::endl;
			return new btBoxShape(shape.dimensions / 2);
		}
		case PHYS_TRIANGLE_MESH: {
			return new btConvexTriangleMeshShape(shape.triangles);
		}
		default: {
			return 0;
		}
    }
}

// Remove dialog if already open
void closeDialog(irr::s32 id) {
	irr::gui::IGUIElement* elem = gui->getRootGUIElement()
		->getElementFromId(id, true);
	if(elem) {
		elem->remove();
	}
}

void closeAllDialogs() {
	closeDialog(GUI_DIALOG_RESOURCES);
}

inline std::string getDirectory(const std::string& filename) {
    return filename.substr(0, filename.find_last_of("\\/"));
}
inline std::string getExtension(const std::string& filename) {
    return filename.substr(filename.find_last_of(".") + 1);
}

void showResourcesDialog() {
	closeDialog(GUI_DIALOG_RESOURCES);

	// Create dialog
	irr::gui::IGUIWindow* dialog = gui->addWindow(GuiBox(10, 60, 200, 300), false, L"Resources", 0, GUI_DIALOG_RESOURCES);

	// Add tree view
	irr::gui::IGUITreeView* tree = gui->addTreeView(GuiRect(5, 25, 195, 295), dialog, GUI_TREE_RESOURCES, true, true, true);

	irr::gui::IGUITreeViewNode* root = tree->getRoot();

	root->addChildBack(L"Test");

}

inline btVector3 irrToBullet(irr::core::vector3df irr) {
	return btVector3(irr.X, irr.Y, irr.Z);
}

void openPhysicsShape(std::string filename) {
	std::string extension = getExtension(filename);

	PhysicsShape& physShape = currProj->physicsShape;

	if(extension == "json") {
		std::ifstream stream(filename);
		Json::Value physData;
		stream >> physData;

		std::string type = physData["type"].asString();

		if(type == "sphere") {
			std::cout << "Physics shape is a sphere." << std::endl;
			physShape.type = PHYS_SPHERE;
			physShape.radius = physData["radius"].asDouble();
			std::cout << "Radius = " << physShape.radius << std::endl;
		}
		else if(type == "box") {
			std::cout << "Physics shape is a box." << std::endl;
			physShape.type = PHYS_BOX;
			Json::Value& dimen = physData["size"];
			if(dimen != Json::nullValue) {
				btScalar allDimen = dimen.asDouble();
				physShape.dimensions = btVector3(allDimen, allDimen, allDimen);
			}
			Json::Value& dimenX = physData["sizeX"];
			if(dimenX != Json::nullValue) {
				physShape.dimensions.setX(dimenX.asDouble());
			}
			Json::Value& dimenY = physData["sizeY"];
			if(dimenY != Json::nullValue) {
				physShape.dimensions.setY(dimenY.asDouble());
			}
			Json::Value& dimenZ = physData["sizeZ"];
			if(dimenZ != Json::nullValue) {
				physShape.dimensions.setZ(dimenZ.asDouble());
			}
			std::cout << "Size = " << "(" << physShape.dimensions.getX() << ", " << physShape.dimensions.getY() << ", " << physShape.dimensions.getZ() << ")" << std::endl;
		}
		else {
			std::cout << "Physics shape is empty." << std::endl;
			physShape.type = PHYS_EMPTY;
		}

	}
	else {
		std::cout << "Physics shape is a mesh." << std::endl;
		physShape.type = PHYS_TRIANGLE_MESH;


		std::cout << "Loading file as mesh..." << std::endl;
		irr::io::path path(filename.c_str());
		irr::scene::IAnimatedMesh* mesh = smgr->getMesh(path);
		irr::scene::IMeshBuffer* buffer = mesh->getMeshBuffer(0);

		const irr::u16* indices = buffer->getIndices();
		irr::u32 indexCount = buffer->getIndexCount();

		std::cout << "Loading trimesh for physics." << std::endl;
		std::cout << "Indices: " << indexCount << std::endl;
		std::cout << "Triangles: " << indexCount / 3 << std::endl;

		btTriangleMesh* triMesh = currProj->physicsShape.triangles = new btTriangleMesh();

		for(irr::u32 i = 0; i < indexCount; i += 3) {
			triMesh->addTriangle(
				irrToBullet(buffer->getPosition(indices[i])),
				irrToBullet(buffer->getPosition(indices[i + 1])),
				irrToBullet(buffer->getPosition(indices[i + 2])),

				// Whether or not to remove duplicate verts
				true
			);
		}
		std::cout << "Physics mesh successfully generated." << std::endl;

	}
	btCollisionShape* shape = newShape(currProj->physicsShape);

	if(shape) {
		bulletWorld->addCollisionObject(new btRigidBody(0, new btDefaultMotionState(), shape, btVector3(0, 0, 0)));
	}

}

void openModel(std::string filename) {
	irr::io::path path(filename.c_str());
	irr::scene::IAnimatedMesh* mesh = smgr->getMesh(path);
	irr::scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh, rootNode);
	node->getMaterial(0).AmbientColor = irr::video::SColor(1, 1, 1, 1);
	node->getMaterial(0).Lighting = false;
}

void openFile(std::string filename) {

	std::ifstream stream(filename);

	if(currProj) {
		delete currProj;
	}
	currProj = new Project();

	currProj->dir = getDirectory(filename);
	std::cout << "Object directory: " << currProj->dir << std::endl;

	stream >> currProj->jsonFile;
	Json::Value& data = currProj->jsonFile;

	Json::Value& physData = data["physics"];
	openPhysicsShape(currProj->dir + "/" + physData.asString());

	Json::Value& modelData = data["model"];
	openModel(currProj->dir + "/" + modelData.asString());

	std::cout << "Object opened:" << std::endl;
	std::cout << data << std::endl;
}




class AppEventReceiver : public irr::IEventReceiver {
public:
	virtual bool OnEvent(const irr::SEvent& event) {

		if(event.EventType == irr::EET_GUI_EVENT) {
			irr::s32 id = event.GUIEvent.Caller->getID();

			switch(event.GUIEvent.EventType) {

				// Menu item selected
				case irr::gui::EGET_MENU_ITEM_SELECTED: {
					irr::gui::IGUIContextMenu* menu = (irr::gui::IGUIContextMenu*) event.GUIEvent.Caller;
					id = menu->getItemCommandId(menu->getSelectedItem());
					switch(id) {
						// File -> Open
						case GUI_FILE_OPEN: {
							gui->addFileOpenDialog(L"Open object...");
							return true;
						}
						default: {
							break;
						}
					}
					break;
				}

				// File selected
				case irr::gui::EGET_FILE_SELECTED: {
					irr::gui::IGUIFileOpenDialog* dialog = (irr::gui::IGUIFileOpenDialog*) event.GUIEvent.Caller;

					openFile(irr::core::stringc(dialog->getFileName()).c_str());


					break;
				}

				// Button clicked
				case irr::gui::EGET_BUTTON_CLICKED: {
					switch(id) {
						// Resource button
						case GUI_BUTTON_RESOURCES: {
							showResourcesDialog();
							return true;
						}
						default: {
							break;
						}
					}
					break;
				}
				default: {
					break;
				}
			}
		}

		return false;
	}
};

int main()
{
	currProj = 0;

	// Get the preferred driver type
	irr::video::E_DRIVER_TYPE driverType = irr::video::EDT_OPENGL; // driverChoiceConsole(); //
	if(driverType == irr::video::EDT_COUNT) { return 1; }

	// Create the irrlicht device
	AppEventReceiver appEventReceiver;
	irr::SIrrlichtCreationParameters params = irr::SIrrlichtCreationParameters();
	params.DriverType = driverType;
	params.WindowSize = irr::core::dimension2d<irr::u32>(800, 600);
	params.Bits = 16;
	params.Fullscreen = false;
	params.Stencilbuffer = true;
	params.Vsync = false;
	params.EventReceiver = &appEventReceiver;
	params.AntiAlias = 2; // "Multisampling"
	irr::IrrlichtDevice* device = createDeviceEx(params);
	if(!device) { return 1; }

	device->setResizable(true);

	// Get the driver and manager
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	gui = device->getGUIEnvironment();

	// Font
	irr::gui::IGUISkin* skin = gui->getSkin();
	irr::gui::IGUIFont* font = gui->getFont("example_media/fonthaettenschweiler.bmp");
	if (font) { skin->setFont(font); }

	broadphase = new btDbvtBroadphase;
	collisionConfiguration = new btDefaultCollisionConfiguration;
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	bulletWorld = new btCollisionWorld(dispatcher, broadphase, collisionConfiguration);
	bulletDebugDrawer = new reib::BulletDebugDrawer(device);
	bulletWorld->setDebugDrawer(bulletDebugDrawer);

	rootNode = smgr->addEmptySceneNode();
	//rootNode->setScale(irr::core::vector3df(10, 10, 10));

	{
		irr::gui::IGUIContextMenu* menu = gui->addMenu();
		menu->addItem(L"File", -1, true, true);
		menu->addItem(L"Edit", -1, true, true);
		menu->addItem(L"Help", -1, true, true);

		irr::gui::IGUIContextMenu* submenu;

		// File
		submenu = menu->getSubMenu(0);
		submenu->addItem(L"New", GUI_FILE_NEW);
		submenu->addItem(L"Open", GUI_FILE_OPEN);
		submenu->addItem(L"Save", GUI_FILE_SAVE);
		submenu->addItem(L"Save as...", GUI_FILE_SAVE_AS);
	}

	{
		irr::gui::IGUIToolBar* bar = gui->addToolBar();

		irr::video::ITexture* image = driver->getTexture("assets/A.png");
		bar->addButton(GUI_BUTTON_RESOURCES, 0, L"Resources", image, 0, false, true);
		bar->addButton(GUI_BUTTON_COMPONENTS, 0, L"Components", image, 0, false, true);
		bar->addButton(GUI_BUTTON_MODEL, 0, L"Model", image, 0, false, true);
		bar->addButton(GUI_BUTTON_PHYSICS, 0, L"Physics", image, 0, false, true);
		bar->addButton(GUI_BUTTON_ANIMATION, 0, L"Animation", image, 0, false, true);


	}

	// Disable alpha
	for(irr::u32 i = 0; i < irr::gui::EGDC_COUNT; ++ i) {
		irr::video::SColor color = gui->getSkin()->getColor((irr::gui::EGUI_DEFAULT_COLOR) i);
		color.setAlpha(255);
		gui->getSkin()->setColor((irr::gui::EGUI_DEFAULT_COLOR) i, color);
	}

	showResourcesDialog();

/*
	// Cool skybox
	driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);
	smgr->addSkyBoxSceneNode(
		driver->getTexture("assets_editor/cloudy_0/bluecloud_up.jpg"),
		driver->getTexture("assets_editor/cloudy_0/bluecloud_dn.jpg"),
		driver->getTexture("assets_editor/cloudy_0/bluecloud_rt.jpg"),
		driver->getTexture("assets_editor/cloudy_0/bluecloud_lf.jpg"),
		driver->getTexture("assets_editor/cloudy_0/bluecloud_ft.jpg"),
		driver->getTexture("assets_editor/cloudy_0/bluecloud_bk.jpg"));
	driver->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, true);
	*/

	irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
	cam->setPosition(irr::core::vector3df(3, 3, 3));
	cam->setTarget(irr::core::vector3df(0, 0, 0));

	smgr->setAmbientLight(irr::video::SColorf(1, 1, 1, 1));

	// Initialize tpf calculator
	irr::u32 then = device->getTimer()->getTime();
	irr::u32 frames = 0;

	// Main loop
	while(device->run()) {
		// Calculate time per frame in seconds
		const irr::u32 now = device->getTimer()->getTime();
        const irr::f32 tpf = (irr::f32)(now - then) / 1000.f;
        then = now;

		// Clear buffers before rendering
		driver->beginScene(true, true, irr::video::SColor(0, 140, 140, 140));

		// Draw stuff
		smgr->drawAll();
		driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
		driver->setMaterial(bulletDebugDrawer->material);
		bulletWorld->debugDrawWorld();
		gui->drawAll();

		// Flush
		driver->endScene();

		// Debug counter
		++ frames;
		if(frames >= 100) {
			irr::core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (irr::s32) driver->getFPS();

			device->setWindowCaption(str.c_str());
			frames = 0;
		}
	}

	device->drop(); device = 0;

	delete bulletWorld;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
	delete bulletDebugDrawer;

	return 0;
}
