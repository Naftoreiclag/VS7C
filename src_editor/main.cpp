/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "irrlicht.h"
#include "driverChoice.h"
#include "json/json.h"
#include "btBulletCollisionCommon.h"
#include "ReiBullet.h"

irr::video::IVideoDriver* driver;
irr::scene::ISceneManager* smgr;
irr::gui::IGUIEnvironment* gui;

irr::gui::IGUIWindow* resourcesDialog;
irr::gui::IGUIWindow* physicsDialog;
irr::gui::IGUIWindow* modelDialog;

irr::scene::ISceneNode* rootNode;

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

    GUI_COMBO_PHYSICS_TYPE,
    GUI_EDIT_PHYSICS_OFFSET_X,
    GUI_EDIT_PHYSICS_OFFSET_Y,
    GUI_EDIT_PHYSICS_OFFSET_Z,
    GUI_EDIT_PHYSICS_DIMENSIONS_X,
    GUI_EDIT_PHYSICS_DIMENSIONS_Y,
    GUI_EDIT_PHYSICS_DIMENSIONS_Z,
    GUI_EDIT_PHYSICS_RADIUS,
    GUI_EDIT_PHYSICS_HEIGHT,
    GUI_EDIT_PHYSICS_FILE,
    GUI_EMPTY_PHYSICS_TYPE_EDITOR,

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
	std::string filename;
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

	btVector3 physicsOffset;
	PhysicsShape physicsShape;
};

Project* currProj;

void removeAllChildren(irr::gui::IGUIElement* item) {
	// Make a copy of the lsit to avoid concurrent modification, and to remove constness
	irr::core::list<irr::gui::IGUIElement*> childrenList = item->getChildren();

	for(irr::core::list<irr::gui::IGUIElement*>::Iterator it = childrenList.begin(); it != childrenList.end(); ++ it) {
		irr::gui::IGUIElement* child = *it;

		child->remove();
	}

}

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
void closeDialog(irr::s32 id) {
	irr::gui::IGUIElement* dialog = gui->getRootGUIElement()
		->getElementFromId(id, true);
	if(dialog) {
		dialog->remove();
	}
}
inline void closeAllDialogs() {
	closeDialog(GUI_DIALOG_RESOURCES);
	resourcesDialog = 0;
	closeDialog(GUI_DIALOG_PHYSICS);
	physicsDialog = 0;
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
	irr::gui::IGUIWindow* dialog = resourcesDialog = gui->addWindow(GuiBox(10, 60, 200, 300), false, L"Resources", 0, GUI_DIALOG_RESOURCES);

	// Add tree view
	irr::gui::IGUITreeView* tree = gui->addTreeView(GuiRect(5, 25, 195, 295), dialog, GUI_TREE_RESOURCES, true, true, true);

	irr::gui::IGUITreeViewNode* root = tree->getRoot();

	root->addChildBack(L"Test");

}
const wchar_t* toText(std::string value) {
	std::wstring ws(value.begin(), value.end());
	return ws.c_str();
}
const wchar_t* toText(btScalar value) {
	std::stringstream ss;
	ss << value;
	return toText(ss.str());
}
const std::string toString(const wchar_t* text) {
	std::stringstream ss;
	for(std::size_t i = 0; i < 99999; ++ i) {
		if(text[i] == 0) {
			break;
		}
		ss << ((char) text[i]);
	}
	return ss.str();
}
btScalar toScalar(const wchar_t* text) {
	std::stringstream ss(toString(text));
	double ret;
	if(!(ss >> ret)) {
		return 0;
	}
	return ret;
}

irr::s32 physicsComboToType(irr::s32 comboInput) {
	switch(comboInput) {
		case 1: { return PHYS_SPHERE; }
		case 2: { return PHYS_BOX; }
		case 3: { return PHYS_CYLINDER; }
		case 4: { return PHYS_CAPSULE; }
		case 5: { return PHYS_CONE; }
		case 6: { return PHYS_MULTI_SPHERE; }
		case 7: { return PHYS_TRIANGLE_MESH; }
		default: { return PHYS_EMPTY; }
	}
}
irr::s32 physicsTypeToCombo(irr::s32 type) {
	switch(type) {
		case PHYS_SPHERE: { return 1; }
		case PHYS_BOX: { return 2; }
		case PHYS_CYLINDER: { return 3; }
		case PHYS_CAPSULE: { return 4; }
		case PHYS_CONE: { return 5; }
		case PHYS_MULTI_SPHERE: { return 6; }
		case PHYS_TRIANGLE_MESH: { return 7; }
		default: { return 0; }

	}
}
inline void updatePhysicsDialog() {
	if(physicsDialog == 0) {
		return;
	}
	irr::gui::IGUIComboBox* typeSel = (irr::gui::IGUIComboBox*) physicsDialog->getElementFromId(GUI_COMBO_PHYSICS_TYPE);
	typeSel->setSelected(physicsTypeToCombo(currProj->physicsShape.type));

	irr::gui::IGUIStaticText* dialog = (irr::gui::IGUIStaticText*) physicsDialog->getElementFromId(GUI_EMPTY_PHYSICS_TYPE_EDITOR);

	removeAllChildren(dialog);

	irr::s32 physType = currProj->physicsShape.type;
	PhysicsShape physShape = currProj->physicsShape;

	if(physType == PHYS_BOX || physType == PHYS_CYLINDER) {
		gui->addStaticText(L"Size:", GuiBox(5, 5, 69, 69), false, true, dialog);
		gui->addStaticText(L"X", GuiBox(65,  5, 69, 69), false, false, dialog);
		gui->addStaticText(L"Y", GuiBox(65, 25, 69, 69), false, false, dialog);
		gui->addStaticText(L"Z", GuiBox(65, 45, 69, 69), false, false, dialog);
		gui->addEditBox(toText(physShape.dimensions.getX()), GuiBox(80,  5, 115, 20), true, dialog, GUI_EDIT_PHYSICS_DIMENSIONS_X);
		gui->addEditBox(toText(physShape.dimensions.getY()), GuiBox(80, 25, 115, 20), true, dialog, GUI_EDIT_PHYSICS_DIMENSIONS_Y);
		gui->addEditBox(toText(physShape.dimensions.getZ()), GuiBox(80, 45, 115, 20), true, dialog, GUI_EDIT_PHYSICS_DIMENSIONS_Z);
	}
	if(physType == PHYS_SPHERE || physType == PHYS_CAPSULE || physType == PHYS_CONE) {
		gui->addStaticText(L"Radius:", GuiBox(5, 5, 69, 69), false, true, dialog);
		gui->addEditBox(toText(physShape.radius), GuiBox(80,  5, 115, 20), true, dialog, GUI_EDIT_PHYSICS_RADIUS);
	}
	if(physType == PHYS_CAPSULE || physType == PHYS_CONE) {
		gui->addStaticText(L"Height:", GuiBox(5, 30, 69, 69), false, true, dialog);
		gui->addEditBox(toText(physShape.height), GuiBox(80, 30, 115, 20), true, dialog, GUI_EDIT_PHYSICS_HEIGHT);
	}
	if(physType == PHYS_TRIANGLE_MESH) {
		gui->addStaticText(L"File:", GuiBox(5, 5, 69, 69), false, true, dialog);
		gui->addEditBox(toText(currProj->physicsShape.filename), GuiBox(5, 30, 190, 20), true, dialog, GUI_EDIT_PHYSICS_FILE);
	}


}

void showPhysicsDialog() {
	closeDialog(GUI_DIALOG_PHYSICS);

	// Create dialog
	irr::gui::IGUIWindow* dialog = physicsDialog = gui->addWindow(GuiBox(595, 60, 200, 300), false, L"Physics", 0, GUI_DIALOG_PHYSICS);

	gui->addStaticText(L"Offset:", GuiBox(5, 25, 69, 69), false, false, dialog);
	gui->addStaticText(L"X", GuiBox(65, 25, 69, 69), false, false, dialog);
	gui->addStaticText(L"Y", GuiBox(65, 45, 69, 69), false, false, dialog);
	gui->addStaticText(L"Z", GuiBox(65, 65, 69, 69), false, false, dialog);
	gui->addEditBox(L"0", GuiBox(80, 25, 115, 20), true, dialog, GUI_EDIT_PHYSICS_OFFSET_X);
	gui->addEditBox(L"0", GuiBox(80, 45, 115, 20), true, dialog, GUI_EDIT_PHYSICS_OFFSET_Y);
	gui->addEditBox(L"0", GuiBox(80, 65, 115, 20), true, dialog, GUI_EDIT_PHYSICS_OFFSET_Z);

	gui->addStaticText(L"Type:", GuiBox(5, 90, 69, 69), false, false, dialog);
	irr::gui::IGUIComboBox* typeSel = gui->addComboBox(GuiBox(80, 90, 115, 20), dialog, GUI_COMBO_PHYSICS_TYPE);
	typeSel->addItem(L"Empty");
	typeSel->addItem(L"Sphere");
	typeSel->addItem(L"Box");
	typeSel->addItem(L"Cylinder");
	typeSel->addItem(L"Capsule");
	typeSel->addItem(L"Cone");
	typeSel->addItem(L"Multi-Sphere");
	typeSel->addItem(L"Trimesh");

	gui->addStaticText(L"", GuiRect(0, 110, 200, 300), false, true, dialog, GUI_EMPTY_PHYSICS_TYPE_EDITOR);

	updatePhysicsDialog();

}
inline void openAllDialogs() {
	showResourcesDialog();
	showPhysicsDialog();
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
			Json::Value& dimenX = physData["size-x"];
			if(dimenX != Json::nullValue) {
				physShape.dimensions.setX(dimenX.asDouble());
			}
			Json::Value& dimenY = physData["size-y"];
			if(dimenY != Json::nullValue) {
				physShape.dimensions.setY(dimenY.asDouble());
			}
			Json::Value& dimenZ = physData["size-z"];
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
	currProj->physicsShape.filename = currProj->dir + "/" + physData.asString();
	openPhysicsShape(currProj->physicsShape.filename);

	Json::Value& modelData = data["model"];
	openModel(currProj->dir + "/" + modelData.asString());

	std::cout << "Object opened:" << std::endl;
	std::cout << data << std::endl;
}

void saveFile() {
	if(currProj->physicsShape.type == PHYS_TRIANGLE_MESH) {

	}
	else {
		PhysicsShape& shape = currProj->physicsShape;

		std::ofstream physicsFile(shape.filename);

		Json::Value jdata;

		Json::Value& jtype = jdata["type"];
		switch(shape.type) {
			case PHYS_SPHERE: {
				jtype = "sphere";
				break;
			}
			case PHYS_BOX: {
				jtype = "box";
				break;
			}
			case PHYS_CYLINDER: {
				jtype = "cylinder";
				break;
			}
			case PHYS_CAPSULE: {
				jtype = "capsule";
				break;
			}
			case PHYS_CONE: {
				jtype = "cone";
				break;
			}
			case PHYS_MULTI_SPHERE: {
				jtype = "multi-sphere";
				break;
			}
			default: {
				jtype = "empty";
				break;
			}
		}

		if(shape.type == PHYS_BOX || shape.type == PHYS_CYLINDER) {
			jdata["size-x"] = shape.dimensions.getX();
			jdata["size-y"] = shape.dimensions.getY();
			jdata["size-z"] = shape.dimensions.getZ();
		}
		if(shape.type == PHYS_CAPSULE || shape.type == PHYS_CONE || shape.type == PHYS_SPHERE) {
			jdata["radius"] = shape.radius;
		}
		if(shape.type == PHYS_CAPSULE || shape.type == PHYS_CONE) {
			jdata["height"] = shape.height;
		}


		physicsFile << jdata;

	}



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
						case GUI_FILE_SAVE: {
							saveFile();
							return true;
						}
						default: {
							break;
						}
					}
					break;
				}

				// Edit box changed
				case irr::gui::EGET_EDITBOX_CHANGED: {
					irr::gui::IGUIEditBox* box = (irr::gui::IGUIEditBox*) event.GUIEvent.Caller;
					const wchar_t* text = box->getText();
					btScalar scalar = toScalar(text);
					std::string str = toString(text);

					switch(id) {
						case GUI_EDIT_PHYSICS_OFFSET_X: {
							currProj->physicsOffset.setX(scalar);
							return true;
						}
						case GUI_EDIT_PHYSICS_OFFSET_Y: {
							currProj->physicsOffset.setY(scalar);
							return true;
						}
						case GUI_EDIT_PHYSICS_OFFSET_Z: {
							currProj->physicsOffset.setZ(scalar);
							return true;
						}
						case GUI_EDIT_PHYSICS_HEIGHT: {
							currProj->physicsShape.height = scalar;
							return true;
						}
						case GUI_EDIT_PHYSICS_DIMENSIONS_X: {
							currProj->physicsShape.dimensions.setX(scalar);
							return true;
						}
						case GUI_EDIT_PHYSICS_DIMENSIONS_Y: {
							currProj->physicsShape.dimensions.setY(scalar);
							return true;
						}
						case GUI_EDIT_PHYSICS_DIMENSIONS_Z: {
							currProj->physicsShape.dimensions.setZ(scalar);
							return true;
						}
						case GUI_EDIT_PHYSICS_RADIUS: {
							std::cout << "Radius changed to " << scalar << "." << std::endl;
							currProj->physicsShape.radius = scalar;
							return true;
						}
					}
					break;
				}

				// Combo box changed
				case irr::gui::EGET_COMBO_BOX_CHANGED: {

					switch(id) {
						case GUI_COMBO_PHYSICS_TYPE: {

							irr::gui::IGUIComboBox* combo = (irr::gui::IGUIComboBox*) event.GUIEvent.Caller;

							currProj->physicsShape.type = physicsComboToType(combo->getSelected());

							updatePhysicsDialog();

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
						case GUI_BUTTON_PHYSICS: {
							showPhysicsDialog();
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

int main() {
	currProj = new Project();

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
	irr::gui::IGUIFont* font = gui->getFont("assets_editor/ubuntu_font/ubuntu-condensed.png");
	if (font) { skin->setFont(font); }

	broadphase = new btDbvtBroadphase;
	collisionConfiguration = new btDefaultCollisionConfiguration;
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	bulletWorld = new btCollisionWorld(dispatcher, broadphase, collisionConfiguration);
	bulletDebugDrawer = new reib::BulletDebugDrawer(device);
	bulletWorld->setDebugDrawer(bulletDebugDrawer);

	btStaticPlaneShape* planeShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btRigidBody* planeRigid = new btRigidBody(0, 0, planeShape);
	bulletWorld->addCollisionObject(planeRigid);
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
		submenu->addSeparator();
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

	openAllDialogs();

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
