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

#include "assimp/Importer.hpp"

#include "irrlicht.h"
#include "driverChoice.h"
#include "json/json.h"
#include "btBulletCollisionCommon.h"

#include "ReiBullet.h"
#include "ReiAssimp.h"

irr::IrrlichtDevice* device;
irr::video::IVideoDriver* driver;
irr::scene::ISceneManager* smgr;
irr::gui::IGUIEnvironment* gui;
irr::video::IGPUProgrammingServices* gpuProg;

irr::gui::IGUIWindow* resourcesDialog;
irr::gui::IGUIWindow* objectsDialog;
irr::gui::IGUIWindow* physicsDialog;
irr::gui::IGUIWindow* modelDialog;
irr::gui::IGUIWindow* animationDialog;
irr::gui::IGUIWindow* morphDialog;

irr::scene::ISceneNode* rootNode;

btBroadphaseInterface* broadphase;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;

btCollisionWorld* bulletWorld;
reib::BulletDebugDrawer* bulletDebugDrawer;

typedef double btDouble;

enum {

	GUI_ROOT_WINDOW = 0x10000,

	GUI_FILE_NEW,
	GUI_FILE_OPEN,
	GUI_FILE_SAVE,
	GUI_FILE_SAVE_AS,

	GUI_DIALOG_RESOURCES,
	GUI_DIALOG_OBJECTS,
	GUI_DIALOG_COMPONENTS,
	GUI_DIALOG_MODEL,
	GUI_DIALOG_PHYSICS,
	GUI_DIALOG_ANIMATION,
	GUI_DIALOG_MORPH,
	GUI_DIALOG_FACIAL,
	GUI_DIALOG_NEWOBJECT,

	GUI_BUTTON_NEWOBJECT_OK,
	GUI_BUTTON_NEWOBJECT_AUTONAME,
	GUI_EDIT_NEWOBJECT_ID,
	GUI_EDIT_NEWOBJECT_LOC,
	GUI_EDIT_NEWOBJECT_MODEL,
	GUI_EDIT_NEWOBJECT_PHYSICS,

	GUI_TREE_RESOURCES,

    GUI_BUTTON_DIALOG_RESOURCES,
    GUI_BUTTON_DIALOG_OBJECTS,
    GUI_BUTTON_DIALOG_COMPONENTS,
    GUI_BUTTON_DIALOG_MODEL,
    GUI_BUTTON_DIALOG_PHYSICS,
    GUI_BUTTON_DIALOG_ANIMATION,
    GUI_BUTTON_DIALOG_MORPH,
    GUI_BUTTON_DIALOG_FACIAL,

	GUI_EDIT_OBJECTS_SEARCH,
	GUI_LIST_OBJECTS,
	GUI_SCROLL_OBJECTS_INFO,
	GUI_EMPTY_OBJECTS_INFO_AREA,
	GUI_EMPTY_OBJECTS_INFO,
	GUI_TEXT_OBJECTS_ID,
	GUI_EDIT_OBJECTS_ID,
	GUI_BUTTON_OBJECTS_NEW,
	GUI_BUTTON_OBJECTS_ADD,
	GUI_BUTTON_OBJECTS_REMOVE,

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
    GUI_BUTTON_PHYSICS_SETBB,
    GUI_BUTTON_PHYSICS_SHRINK,
    GUI_BUTTON_PHYSICS_GROW,
    GUI_EMPTY_PHYSICS_TYPE_EDITOR,

    GUI_COMBO_ANIM,
    GUI_BUTTON_ANIM_PLAY,
    GUI_BUTTON_ANIM_STOP,

	STATE_EDIT,
	STATE_PAN,
	STATE_LOOK,

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

irr::video::E_MATERIAL_TYPE shaderMatType;
irr::core::position2di screenCenter;
irr::s32 appState = STATE_EDIT;

// Represents a typical physics shape
struct PhysicsShape {
	// Is this file modified, does not apply for non-json files
	bool modified = false;

	// Original json file, if applicable
	Json::Value jVal = Json::nullValue;

	// Where is the file located
	std::string filename = "";

	// Type
	irr::s32 type = PHYS_EMPTY;

	btDouble radius = 0; // Sphere, Capsule, Cone
	btVector3 dimensions = btVector3(0, 0, 0); // Box, Cylinder
	btDouble height = 0; // Capsule, Cone

	std::vector<std::pair<btVector3, btDouble>> locRadi; // MULTI_SPHERE
	btTriangleMesh* triangles = 0; // TRIANGLE_MESH
};

// Represents a typical object.json file pointed to by content-pack.json
struct Gobject {
	// Is this file modified
	bool modified = false;

	// Original json file
	Json::Value jVal = Json::nullValue;

	// Where is the file located
	std::string filename = "null";

	// Unique id
	std::string id = "null";

	// Offset from the origin
	btVector3 physicsOffset = btVector3(0, 0, 0);

	//
	bool modelFileExists = true;

	//
	std::string physicsFile = "null";
	std::string modelFile = "null";

	// The shape as described by physicsFile
	PhysicsShape physicsShape;

	//
	std::vector<irr::s32> shaders;

	// For rendering only
	btCollisionShape* collShape = 0;
	btCollisionObject* collObj = 0;
	irr::scene::ISceneNode* sceneNode = 0;
	irr::scene::IAnimatedMesh* bulletTriMesh = 0;
	irr::scene::IAnimatedMesh* sceneMesh = 0;
};

// Represents the content-pack.json file located at root of content pack
struct Cpack {
	// Is this file modified
	bool modified = false;

	// Original json file
	Json::Value jVal = Json::nullValue;

	// Properties
	std::string name = "null";
	std::string root = "null";
	std::string nmsp = "null";
	std::string desc = "null";

	std::vector<Gobject*> gobjectFiles;
};

Cpack* loadedPack;
Gobject* openedObject;

// UTILITY
// =======
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
inline std::string getDirectory(const std::string& filename) {
    return filename.substr(0, filename.find_last_of("\\/"));
}
inline std::string getExtension(const std::string& filename) {
    return filename.substr(filename.find_last_of(".") + 1);
}
const wchar_t* toText(std::string value) {
	std::wstring ws(value.begin(), value.end());
	return ws.c_str();
}
const wchar_t* toText(btDouble value) {
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
btDouble toScalar(const wchar_t* text) {
	std::stringstream ss(toString(text));
	double ret;
	if(!(ss >> ret)) {
		return 0;
	}
	return ret;
}
inline btVector3 toBullet(irr::core::vector3df irr) {
	return btVector3(irr.X, irr.Y, irr.Z);
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

std::string toString(irr::video::SColor color) {
	std::stringstream ss;

	ss << "(";
	ss << color.getAlpha();
	ss << ", ";
	ss << color.getRed();
	ss << ", ";
	ss << color.getGreen();
	ss << ", ";
	ss << color.getBlue();
	ss << ")";

	return ss.str();
}
void debugMaterial(irr::video::SMaterial material) {
	std::cout << "Ambient: " << toString(material.AmbientColor) << std::endl;
	std::cout << "Anti Alias: " << material.AntiAliasing << std::endl;
	std::cout << "Backface: " << material.BackfaceCulling << std::endl;
	std::cout << "Blend: " << material.BlendOperation << std::endl;
	std::cout << "Color Mask: " << material.ColorMask << std::endl;
	std::cout << "Color Material: " << (material.ColorMaterial + 0) << std::endl;
	std::cout << "Diffuse: " << toString(material.DiffuseColor) << std::endl;
	std::cout << "Emissive: " << toString(material.EmissiveColor) << std::endl;
	std::cout << "Fog: " << material.FogEnable << std::endl;
	std::cout << "Frontface: " << material.FrontfaceCulling << std::endl;
	std::cout << "Gourad: " << material.GouraudShading << std::endl;
	std::cout << "Light: " << material.Lighting << std::endl;
	std::cout << "Mat type: " << material.MaterialType << std::endl;
	std::cout << "Normalize: " << material.NormalizeNormals << std::endl;
	std::cout << "Shiny: " << material.Shininess << std::endl;
	std::cout << "Specular: " << toString(material.SpecularColor) << std::endl;
	std::cout << "Thickness: " << material.Thickness << std::endl;
	std::cout << "Wireframe: " << material.Wireframe << std::endl;
	std::cout << "ZBuffer: " << material.ZBuffer << std::endl;
}


// 3D DATA MANIPULATION
// ====================
btCollisionShape* newShape(PhysicsShape shape) {
    switch(shape.type) {
		case PHYS_SPHERE: {
			return new btSphereShape(shape.radius);
		}
		case PHYS_BOX: {
			std::cout << "Generated shape from box." << std::endl;
			return new btBoxShape(shape.dimensions / 2);
		}
		case PHYS_CYLINDER: {
			std::cout << "Generated shape from cylinder." << std::endl;
			return new btCylinderShape(shape.dimensions / 2);
		}
		case PHYS_TRIANGLE_MESH: {
			return new btConvexTriangleMeshShape(shape.triangles);
		}
		default: {
			return 0;
		}
    }
}
void physicsGrow(PhysicsShape& physShape, btDouble amount) {
	irr::s32 physType = openedObject->physicsShape.type;

	if(physType == PHYS_BOX || physType == PHYS_CYLINDER) {
		physShape.dimensions *= amount;
	}
	if(physType == PHYS_SPHERE || physType == PHYS_CAPSULE || physType == PHYS_CONE) {
		physShape.radius *= amount;
	}
	if(physType == PHYS_CAPSULE || physType == PHYS_CONE) {
		physShape.height *= amount;
	}
}

// GUI
// ===
void closeDialog(irr::s32 id) {
	irr::gui::IGUIElement* dialog = gui->getRootGUIElement()
		->getElementFromId(id, true);
	if(dialog) {
		dialog->remove();
	}
}
void updateResourcesDialog() {
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

void updateObjectsDialogOnReload() {
	if(objectsDialog == 0) {
		return;
	}
	irr::gui::IGUIListBox* listbox = (irr::gui::IGUIListBox*) objectsDialog->getElementFromId(GUI_LIST_OBJECTS);

	listbox->clear();

	for(std::vector<Gobject*>::iterator it = loadedPack->gobjectFiles.begin(); it != loadedPack->gobjectFiles.end(); ++ it) {
		Gobject* obj = *it;
		listbox->addItem(toText(obj->id));
	}

}
void updateObjectsDialogOnScroll() {
	if(objectsDialog == 0) {
		return;
	}

	irr::gui::IGUIScrollBar* scroll = (irr::gui::IGUIScrollBar*) objectsDialog->getElementFromId(GUI_SCROLL_OBJECTS_INFO);
	irr::gui::IGUIStaticText* objInfo = (irr::gui::IGUIStaticText*) objectsDialog->getElementFromId(GUI_EMPTY_OBJECTS_INFO, true);

	objInfo->setRelativePosition(GuiBox(0, -scroll->getPos(), 1000, 1000));

}
void showObjectsDialog() {
	closeDialog(GUI_DIALOG_OBJECTS);

	objectsDialog = gui->addWindow(GuiBox(10, 60, 420, 500), false, L"Objects", 0, GUI_DIALOG_OBJECTS);

	gui->addEditBox(L"", GuiBox(5, 50, 190, 20), true, objectsDialog, GUI_EDIT_OBJECTS_SEARCH);
	gui->addButton(GuiBox(5, 25, 60, 20), objectsDialog, GUI_BUTTON_OBJECTS_NEW, L"New", L"Create and add new object");
	gui->addButton(GuiBox(70, 25, 60, 20), objectsDialog, GUI_BUTTON_OBJECTS_NEW, L"Add", L"Add object to list");
	gui->addButton(GuiBox(135, 25, 60, 20), objectsDialog, GUI_BUTTON_OBJECTS_NEW, L"Remove", L"Remove object from list");
	gui->addListBox(GuiRect(5, 75, 195, 495), objectsDialog, GUI_LIST_OBJECTS, true);
	irr::gui::IGUIStaticText* objInfoArea = gui->addStaticText(L"", GuiRect(200, 25, 395, 495), false, true, objectsDialog, GUI_EMPTY_OBJECTS_INFO_AREA);

	irr::gui::IGUIStaticText* objInfo = gui->addStaticText(L"", GuiBox(0, 0, 1000, 1000), false, true, objInfoArea, GUI_EMPTY_OBJECTS_INFO);

	gui->addStaticText(L"ID", GuiBox(0, 0, 969, 69), false, false, objInfo, GUI_TEXT_OBJECTS_ID);
	gui->addEditBox(L"", GuiBox(0, 20, 190, 20), true, objInfo, GUI_EDIT_OBJECTS_ID);

	gui->addScrollBar(false, GuiRect(395, 25, 415, 495), objectsDialog, GUI_SCROLL_OBJECTS_INFO);

	updateObjectsDialogOnReload();
}

void updateAnimationDialog() {
}
void showAnimationDialog() {
    closeDialog(GUI_DIALOG_ANIMATION);

    animationDialog = gui->addWindow(GuiBox(10, 60, 400, 200), false, L"Animation", 0, GUI_DIALOG_ANIMATION);

    gui->addComboBox(GuiBox(5, 25, 100, 20), animationDialog, GUI_COMBO_ANIM);
    gui->addButton(GuiBox(110, 25, 60, 20), animationDialog, GUI_BUTTON_ANIM_PLAY, L"Play", L"Plays animation.");
    //gui->addButton()
}

void updatePhysicsDialog() {
	if(physicsDialog == 0) {
		return;
	}
	physicsDialog->getElementFromId(GUI_EDIT_PHYSICS_OFFSET_X)->setText(toText(openedObject->physicsOffset.getX()));
	physicsDialog->getElementFromId(GUI_EDIT_PHYSICS_OFFSET_Y)->setText(toText(openedObject->physicsOffset.getY()));
	physicsDialog->getElementFromId(GUI_EDIT_PHYSICS_OFFSET_Z)->setText(toText(openedObject->physicsOffset.getZ()));

	irr::gui::IGUIComboBox* typeSel = (irr::gui::IGUIComboBox*) physicsDialog->getElementFromId(GUI_COMBO_PHYSICS_TYPE);
	typeSel->setSelected(physicsTypeToCombo(openedObject->physicsShape.type));

	irr::gui::IGUIStaticText* dialog = (irr::gui::IGUIStaticText*) physicsDialog->getElementFromId(GUI_EMPTY_PHYSICS_TYPE_EDITOR);

	removeAllChildren(dialog);

	irr::s32 physType = openedObject->physicsShape.type;
	PhysicsShape physShape = openedObject->physicsShape;

	if(physType != PHYS_EMPTY && physType != PHYS_TRIANGLE_MESH) {
		gui->addButton(GuiBox(5, 130, 80, 20), dialog, GUI_BUTTON_PHYSICS_SETBB, L"Auto-size", L"Automatically scales shape.");
		gui->addButton(GuiBox(5, 155, 60, 20), dialog, GUI_BUTTON_PHYSICS_SHRINK, L"Shrink", L"Contract");
		gui->addButton(GuiBox(70, 155, 60, 20), dialog, GUI_BUTTON_PHYSICS_GROW, L"Grow", L"Expand");
	}
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
		gui->addEditBox(toText(openedObject->physicsShape.filename), GuiBox(5, 30, 190, 20), true, dialog, GUI_EDIT_PHYSICS_FILE);
	}


}
void showPhysicsDialog() {
	closeDialog(GUI_DIALOG_PHYSICS);

	// Create dialog
	irr::gui::IGUIWindow* dialog = physicsDialog = gui->addWindow(GuiBox(10, 60, 200, 300), false, L"Physics", 0, GUI_DIALOG_PHYSICS);

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

void openAllDialogs() {
	/*
	showResourcesDialog();
	showObjectsDialog();
	showPhysicsDialog();
	*/
}
void closeAllDialogs() {
	closeDialog(GUI_DIALOG_RESOURCES);
	resourcesDialog = 0;
	closeDialog(GUI_DIALOG_OBJECTS);
	objectsDialog = 0;
	closeDialog(GUI_DIALOG_PHYSICS);
	physicsDialog = 0;
	closeDialog(GUI_DIALOG_ANIMATION);
	animationDialog = 0;
	closeDialog(GUI_DIALOG_MORPH);
	morphDialog = 0;
}

// DRAWING
// =======
void updatePhysicsRendering() {
	if(openedObject->collObj) {
		bulletWorld->removeCollisionObject(openedObject->collObj);
		delete openedObject->collObj;
	}

	openedObject->collShape = newShape(openedObject->physicsShape);

	if(openedObject->collShape) {
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(openedObject->physicsOffset);
		openedObject->collObj = new btRigidBody(0, new btDefaultMotionState(trans), openedObject->collShape, btVector3(0, 0, 0));
		bulletWorld->addCollisionObject(openedObject->collObj);
	}
}

// IO NOT THE MOON
// ===============

std::string parseFilename(std::string filename, Gobject* context = 0) {
	std::ifstream test1(filename);
	if(test1.is_open()) {
		return filename;
	}

	if(openedObject) {
		std::ifstream test3(getDirectory(openedObject->filename) + "/" + filename);
		if(test3.is_open()) {
			return getDirectory(openedObject->filename) + "/" + filename;
		}
	}

	if(loadedPack) {
		std::ifstream test2(loadedPack->root + "/" + filename);
		if(test2.is_open()) {
			return loadedPack->root + "/" + filename;
		}
	}

	if(context) {
		std::ifstream test4(getDirectory(context->filename) + "/" + filename);
		if(test4.is_open()) {
			return getDirectory(context->filename) + "/" + filename;
		}
	}

	if(loadedPack) {
		return loadedPack->root + "/" + filename;
	}
	else {
		return filename;
	}

}

Json::Value toJson(btVector3 vector) {

	btDouble x = vector.getX();
	btDouble y = vector.getY();
	btDouble z = vector.getZ();

	if(x == y && x == z) {
		Json::Value ret = x;
		return ret;
	}
	else {
		Json::Value ret = Json::arrayValue;

		ret[0] = x;
		ret[1] = y;
		ret[2] = z;

		return ret;
	}
}

btVector3 toBullet(Json::Value& jValue) {
	if(jValue.isArray()) {
		btVector3 retVal(0, 0, 0);

		Json::Value& jx = jValue[0];
		Json::Value& jy = jValue[1];
		Json::Value& jz = jValue[2];

		if(jx.isDouble()) {
			retVal.setX(jx.asDouble());
		}
		if(jy.isDouble()) {
			retVal.setY(jy.asDouble());
		}
		if(jz.isDouble()) {
			retVal.setZ(jz.asDouble());
		}

		return retVal;
	}
	if(jValue.isDouble()) {
		btDouble n = jValue.asDouble();
		return btVector3(n, n, n);
	} else {
		return btVector3(0, 0, 0);
	}
}

void createFileIfNotExist(std::string filename) {
	std::ifstream stream(filename);

	std::cout << "Checking that " << filename << " exists..." << std::endl;
	std::cout << "File does " << (stream.is_open() ? "" : "not ") << "exist." << std::endl;
	if(!stream.is_open()) {

		irr::io::IFileSystem* fs = device->getFileSystem();

		fs->createAndOpenFile(irr::io::path(filename.c_str()));
		//fs->createFileList(,)

	}
}

void openAndRenderPhysicsShape(std::string filename) {
	filename = parseFilename(filename);
	std::cout << filename << std::endl;

	PhysicsShape& physShape = openedObject->physicsShape;

	std::string extension = getExtension(filename);
	if(extension == "json") {
		std::ifstream stream(filename);
		Json::Value jphysData;
		stream >> jphysData;

		std::string type = jphysData["type"].asString();

		bool isKnown = false;

		// Load a sphere from the json file
		if(type == "sphere") {
			// Set type to be a sphere
			physShape.type = PHYS_SPHERE;

			// Set radius
			physShape.radius = jphysData["radius"].asDouble();
			std::cout << "Radius = " << physShape.radius << std::endl;

			isKnown = true;
		}

		if(type == "box" || type == "cylinder") {
			if(type == "box") {
				physShape.type = PHYS_BOX;
			}
			else if(type == "cylinder") {
				physShape.type = PHYS_CYLINDER;
			}

			physShape.dimensions = toBullet(jphysData["size"]);
			std::cout << "Size = " << "(" << physShape.dimensions.getX() << ", " << physShape.dimensions.getY() << ", " << physShape.dimensions.getZ() << ")" << std::endl;

			isKnown = true;
		}

		if(!isKnown)
		{
			std::cout << "Physics shape is empty." << std::endl;
			physShape.type = PHYS_EMPTY;
		}

	}
	else {
		std::cout << "Physics shape is a mesh." << std::endl;
		physShape.type = PHYS_TRIANGLE_MESH;


		std::cout << "Loading file as mesh..." << std::endl;
		irr::io::path path(filename.c_str());
		openedObject->bulletTriMesh = smgr->getMesh(path);
		irr::scene::IMeshBuffer* buffer = openedObject->bulletTriMesh->getMeshBuffer(0);

		const irr::u16* indices = buffer->getIndices();
		irr::u32 indexCount = buffer->getIndexCount();

		std::cout << "Loading trimesh for physics." << std::endl;
		std::cout << "Indices: " << indexCount << std::endl;
		std::cout << "Triangles: " << indexCount / 3 << std::endl;

		btTriangleMesh* triMesh = openedObject->physicsShape.triangles = new btTriangleMesh();

		for(irr::u32 i = 0; i < indexCount; i += 3) {
			triMesh->addTriangle(
				toBullet(buffer->getPosition(indices[i])),
				toBullet(buffer->getPosition(indices[i + 1])),
				toBullet(buffer->getPosition(indices[i + 2])),

				// Whether or not to remove duplicate verts
				true
			);
		}
		std::cout << "Physics mesh successfully generated." << std::endl;

	}

	updatePhysicsDialog();
	updatePhysicsRendering();
}
void openAndRenderModel(std::string filename) {
	filename = parseFilename(filename);
	irr::io::path path(filename.c_str());
	openedObject->sceneMesh = smgr->getMesh(path);
	openedObject->sceneNode = smgr->addAnimatedMeshSceneNode(openedObject->sceneMesh, rootNode);
	irr::video::SMaterial& mat = openedObject->sceneNode->getMaterial(0);

	mat.AmbientColor = irr::video::SColor(255, 255, 255, 255);
	mat.MaterialType = shaderMatType;

	std::cout << "============" << std::endl;
	debugMaterial(mat);
	//ground->getMaterial(0) = mat;


	//mat = ground->getMaterial(0);
}
void closeObject() {
	if(openedObject == 0) {
		return;
	}
	if(openedObject->collObj) {
		std::cout << "Close collision object" << std::endl;
		bulletWorld->removeCollisionObject(openedObject->collObj);
		delete openedObject->collObj;
		openedObject->collObj = 0;
	}
	if(openedObject->collShape) {
		std::cout << "Delete collision shape" << std::endl;
		delete openedObject->collShape;
		openedObject->collShape = 0;
	}
	if(openedObject->sceneNode) {
		std::cout << "Remove scene node" << std::endl;
		openedObject->sceneNode->remove();
		openedObject->sceneNode = 0;
	}
	openedObject = 0;
}
void openObject(Gobject* gobject) {
	closeObject();
	openedObject = gobject;
	openAndRenderModel(gobject->modelFile);
	openAndRenderPhysicsShape(gobject->physicsFile);
}
void saveObject(Gobject* gobj) {
	if(gobj->modified) {
		std::cout << "Saving object: " << parseFilename(gobj->filename) << std::endl;
		gobj->jVal["id"] = gobj->id;
		gobj->jVal["model"] = gobj->modelFile;
		gobj->jVal["physics"] = gobj->physicsFile;
		gobj->jVal["physics-offset"] = toJson(gobj->physicsOffset);

		createFileIfNotExist(parseFilename(gobj->filename));
		std::ofstream objFile(parseFilename(gobj->filename));
		objFile << gobj->jVal;

		gobj->modified = false;
	}

	if(!gobj->modelFileExists) {
		std::cout << "Copied default cube shape" << std::endl;
		std::ifstream defCube("assets_editor/default-cube.dae");
		createFileIfNotExist(parseFilename(gobj->modelFile, gobj));
		std::ofstream newCube(parseFilename(gobj->modelFile, gobj));

		newCube << defCube.rdbuf();

		gobj->modelFileExists = true;

	}

	if(gobj->physicsShape.modified) {
		std::cout << "Saving shape: " << parseFilename(gobj->physicsFile, gobj) << std::endl;
		PhysicsShape& shape = gobj->physicsShape;

		Json::Value& jtype = shape.jVal["type"];
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
			shape.jVal["size"] = toJson(shape.dimensions);
		}
		if(shape.type == PHYS_CAPSULE || shape.type == PHYS_CONE || shape.type == PHYS_SPHERE) {
			shape.jVal["radius"] = shape.radius;
		}
		if(shape.type == PHYS_CAPSULE || shape.type == PHYS_CONE) {
			shape.jVal["height"] = shape.height;
		}

		createFileIfNotExist(parseFilename(gobj->physicsFile, gobj));
		std::ofstream physFile(parseFilename(gobj->physicsFile, gobj));
		physFile << shape.jVal;

		gobj->physicsShape.modified = false;
	}
}
Gobject* loadObject(std::string filename) {
	filename = parseFilename(filename);
	Gobject* retVal = new Gobject();

	retVal->filename = filename;

	std::ifstream stream(filename);
	stream >> retVal->jVal;
	Json::Value& jdata = retVal->jVal;

	retVal->id = retVal->jVal["id"].asString();

	retVal->physicsOffset = toBullet(jdata["physics-offset"]);
	retVal->physicsShape.filename = jdata["physics"].asString();

	retVal->physicsFile = jdata["physics"].asString();
	retVal->modelFile = jdata["model"].asString();

	return retVal;
}
void loadPack(std::string filename) {

	// Unload old pack
	if(loadedPack) {
		delete loadedPack;
	}

	// Open this new pack
	loadedPack = new Cpack();
	loadedPack->root = getDirectory(filename);

	// Begin streaming file
	std::ifstream stream(filename);
	stream >> loadedPack->jVal;

	// Get data from file
	loadedPack->name = loadedPack->jVal["name"].asString();
	loadedPack->desc = loadedPack->jVal["description"].asString();
	loadedPack->nmsp = loadedPack->jVal["namespace"].asString();

	// Load all objects
	Json::Value& jGobjs = loadedPack->jVal["objects"];
	if(jGobjs.isArray()) {
		for(Json::Value::iterator it = jGobjs.begin(); it != jGobjs.end(); ++ it) {
			Json::Value& jGobjFilename = *it;
			Gobject* objData = loadObject(jGobjFilename.asString());
			loadedPack->gobjectFiles.push_back(objData);
		}
	}

	std::cout << "open dialog" << std::endl;
	showObjectsDialog();
}
void saveAll() {
	if(loadedPack->modified) {
		std::cout << "Saving pack info" << std::endl;
		loadedPack->jVal["name"] = loadedPack->name;
		loadedPack->jVal["description"] = loadedPack->desc;
		loadedPack->jVal["namespace"] = loadedPack->nmsp;

		std::ofstream packFile(loadedPack->root + "/content-pack.json");
		packFile << loadedPack->jVal;
	}
	for(std::vector<Gobject*>::iterator it = loadedPack->gobjectFiles.begin(); it != loadedPack->gobjectFiles.end(); ++ it) {
		Gobject* gobj = *it;

		saveObject(gobj);
	}
}

// APPEVENTRECEIVER
// ================
irr::core::position2di mouseLoc;
irr::f32 wheelDelta = 0;
bool mouseJustClicked = false;
bool moveCamToggle = false;
class AppEventReceiver : public irr::IEventReceiver {
public:
	virtual bool OnEvent(const irr::SEvent& event) {

		if(event.EventType == irr::EET_KEY_INPUT_EVENT) {
			if(event.KeyInput.Key == 160) {
				moveCamToggle = event.KeyInput.PressedDown;
			}
			if(event.KeyInput.Key == 27) {
				moveCamToggle = false;
				appState = STATE_EDIT;
			}
			std::cout << "Key pressed: " << event.KeyInput.Key << std::endl;
		}

		if(event.EventType == irr::EET_MOUSE_INPUT_EVENT) {

			if(moveCamToggle) {
				switch(event.MouseInput.Event) {
					case irr::EMIE_LMOUSE_PRESSED_DOWN: {
						appState = STATE_LOOK;
						mouseJustClicked = true;
						break;
					}
					case irr::EMIE_RMOUSE_PRESSED_DOWN: {
						appState = STATE_PAN;
						mouseJustClicked = true;
						break;
					}
					case irr::EMIE_LMOUSE_LEFT_UP: {
						appState = STATE_EDIT;
						break;
					}
					case irr::EMIE_RMOUSE_LEFT_UP: {
						appState = STATE_EDIT;
						break;
					}
					default: {
						break;
					}
				}
			}
			switch(event.MouseInput.Event) {
				case irr::EMIE_MOUSE_MOVED: {
					mouseLoc.X = event.MouseInput.X;
					mouseLoc.Y = event.MouseInput.Y;
					break;
				}
				case irr::EMIE_MOUSE_WHEEL: {
					wheelDelta += event.MouseInput.Wheel;
					break;
				}
				default: {
					break;
				}
			}


		}

		if(event.EventType == irr::EET_GUI_EVENT) {
			// The bane of prank calling
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
							saveAll();
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
					btDouble scalar = toScalar(text);
					std::string str = toString(text);

					switch(id) {
						case GUI_EDIT_PHYSICS_OFFSET_X: {
							openedObject->physicsOffset.setX(scalar);
							openedObject->modified = true;
							updatePhysicsRendering();
							return true;
						}
						case GUI_EDIT_PHYSICS_OFFSET_Y: {
							openedObject->physicsOffset.setY(scalar);
							openedObject->modified = true;
							openedObject->modified = true;
							updatePhysicsRendering();
							return true;
						}
						case GUI_EDIT_PHYSICS_OFFSET_Z: {
							openedObject->physicsOffset.setZ(scalar);
							openedObject->modified = true;
							updatePhysicsRendering();
							return true;
						}
						case GUI_EDIT_PHYSICS_HEIGHT: {
							openedObject->physicsShape.height = scalar;
							openedObject->physicsShape.modified = true;
							updatePhysicsRendering();
							return true;
						}
						case GUI_EDIT_PHYSICS_DIMENSIONS_X: {
							openedObject->physicsShape.dimensions.setX(scalar);
							openedObject->physicsShape.modified = true;
							updatePhysicsRendering();
							return true;
						}
						case GUI_EDIT_PHYSICS_DIMENSIONS_Y: {
							openedObject->physicsShape.dimensions.setY(scalar);
							openedObject->physicsShape.modified = true;
							updatePhysicsRendering();
							return true;
						}
						case GUI_EDIT_PHYSICS_DIMENSIONS_Z: {
							openedObject->physicsShape.dimensions.setZ(scalar);
							openedObject->physicsShape.modified = true;
							updatePhysicsRendering();
							return true;
						}
						case GUI_EDIT_PHYSICS_RADIUS: {
							openedObject->physicsShape.radius = scalar;
							openedObject->physicsShape.modified = true;
							updatePhysicsRendering();
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

							openedObject->physicsShape.type = physicsComboToType(combo->getSelected());

							updatePhysicsDialog();
							updatePhysicsRendering();
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

					loadPack(irr::core::stringc(dialog->getFileName()).c_str());


					break;
				}

				//
				case irr::gui::EGET_LISTBOX_CHANGED: {

					switch(id) {
						case GUI_LIST_OBJECTS: {
							if(loadedPack == 0) {
								break;
							}

							irr::gui::IGUIListBox* listbox = (irr::gui::IGUIListBox*) event.GUIEvent.Caller;

							irr::s32 selectedIndex = listbox->getSelected();

							if(selectedIndex != -1) {
								Gobject* selected = loadedPack->gobjectFiles[selectedIndex];

								openObject(selected);

								// populate
							}
							else {

							}
							break;
						}
					}
				}

				//
				case irr::gui::EGET_SCROLL_BAR_CHANGED: {
					switch(id) {
						case GUI_SCROLL_OBJECTS_INFO: {
							updateObjectsDialogOnScroll();
							break;
						}
					}
				}

				// Button clicked
				case irr::gui::EGET_BUTTON_CLICKED: {
					switch(id) {
						// Resource button
						case GUI_BUTTON_DIALOG_RESOURCES: {
							showResourcesDialog();
							return true;
						}
						case GUI_BUTTON_DIALOG_OBJECTS: {
							showObjectsDialog();
							return true;
						}
						case GUI_BUTTON_DIALOG_PHYSICS: {
							showPhysicsDialog();
							return true;
						}
						case GUI_BUTTON_DIALOG_ANIMATION: {
							showAnimationDialog();
							return true;
						}
						case GUI_BUTTON_PHYSICS_SETBB: {
                            if(!openedObject) {
								break;
                            }
							irr::core::aabbox3df bb = openedObject->sceneNode->getBoundingBox();

							openedObject->physicsOffset = toBullet(bb.getCenter());
							irr::s32 physType = openedObject->physicsShape.type;
							if(physType == PHYS_BOX || physType == PHYS_CYLINDER) {
								openedObject->physicsShape.dimensions = toBullet(bb.getExtent());
							}
							if(physType == PHYS_SPHERE || physType == PHYS_CAPSULE || physType == PHYS_CONE) {
								irr::core::vector3df vec = bb.getExtent();
								vec /= 2;
								openedObject->physicsShape.radius = vec.getLength();
							}
							if(physType == PHYS_CAPSULE || physType == PHYS_CONE) {
								openedObject->physicsShape.height = bb.getExtent().Y;
							}
							openedObject->modified = true;
							openedObject->physicsShape.modified = true;


							updatePhysicsDialog();
							updatePhysicsRendering();
							return true;
						}
						case GUI_BUTTON_PHYSICS_SHRINK: {
                            if(!openedObject) {
								break;
                            }
							physicsGrow(openedObject->physicsShape, 0.95);
							openedObject->physicsShape.modified = true;
							updatePhysicsDialog();
							updatePhysicsRendering();
							return true;
						}
						case GUI_BUTTON_PHYSICS_GROW: {
                            if(!openedObject) {
								break;
                            }
							physicsGrow(openedObject->physicsShape, 1 / 0.95);
							openedObject->physicsShape.modified = true;
							updatePhysicsDialog();
							updatePhysicsRendering();
							return true;
						}
						case GUI_BUTTON_OBJECTS_NEW: {
							closeDialog(GUI_DIALOG_NEWOBJECT);

							irr::u32 width = 350;
							irr::u32 height = 150;

							irr::gui::IGUIWindow* newObjectDialog = gui->addWindow(GuiBox(screenCenter.X - width / 2, screenCenter.Y - height / 2, width, height), false, L"New Object", 0, GUI_DIALOG_NEWOBJECT);

							gui->addButton(GuiBox(width / 2 - 40, height - 25, 80, 20), newObjectDialog, GUI_BUTTON_NEWOBJECT_OK, L"OK", L"Confirm new object");

							gui->addStaticText(L"ID:", GuiBox(5, 25, 69, 69), false, false, newObjectDialog);
							gui->addEditBox(L"", GuiBox(70, 25, width - 135, 20), true, newObjectDialog, GUI_EDIT_NEWOBJECT_ID);
							gui->addButton(GuiBox(width - 70, 25, 65, 20), newObjectDialog, GUI_BUTTON_NEWOBJECT_AUTONAME, L"Rename", L"Automatically picks filename based on id.");

							gui->addStaticText(L"Location:", GuiBox(5, 50, 69, 69), false, false, newObjectDialog);
							gui->addEditBox(L"", GuiBox(70, 50, width - 75, 20), true, newObjectDialog, GUI_EDIT_NEWOBJECT_LOC);
							gui->addStaticText(L"Model:", GuiBox(5, 75, 69, 69), false, false, newObjectDialog);
							gui->addEditBox(L"", GuiBox(70, 75, width - 75, 20), true, newObjectDialog, GUI_EDIT_NEWOBJECT_MODEL);
							gui->addStaticText(L"Physics:", GuiBox(5, 100, 69, 69), false, false, newObjectDialog);
							gui->addEditBox(L"", GuiBox(70, 100, width - 75, 20), true, newObjectDialog, GUI_EDIT_NEWOBJECT_PHYSICS);

							return true;
						}
						case GUI_BUTTON_NEWOBJECT_AUTONAME: {

							irr::gui::IGUIWindow* newObjectDialog = (irr::gui::IGUIWindow*) gui->getRootGUIElement()->getElementFromId(GUI_DIALOG_NEWOBJECT, true);

							irr::gui::IGUIEditBox* editId = (irr::gui::IGUIEditBox*) newObjectDialog->getElementFromId(GUI_EDIT_NEWOBJECT_ID);
							irr::gui::IGUIEditBox* editLoc = (irr::gui::IGUIEditBox*) newObjectDialog->getElementFromId(GUI_EDIT_NEWOBJECT_LOC);
							irr::gui::IGUIEditBox* editModel = (irr::gui::IGUIEditBox*) newObjectDialog->getElementFromId(GUI_EDIT_NEWOBJECT_MODEL);
							irr::gui::IGUIEditBox* editPhys = (irr::gui::IGUIEditBox*) newObjectDialog->getElementFromId(GUI_EDIT_NEWOBJECT_PHYSICS);

							std::string id = toString(editId->getText());

							std::string loc   = id + "/" + id + "-object.json";
							std::string model = id + "/" + id + "-model.dae";
							std::string phys  = id + "/" + id + "-physics.json";

							editLoc->setText(toText(loc));
							editModel->setText(toText(model));
							editPhys->setText(toText(phys));

							return true;
						}
						case GUI_BUTTON_NEWOBJECT_OK: {

							if(loadedPack == 0) {
								std::cout << "Error! no project open!" << std::endl;
								return true;
							}

							irr::gui::IGUIWindow* newObjectDialog = (irr::gui::IGUIWindow*) gui->getRootGUIElement()->getElementFromId(GUI_DIALOG_NEWOBJECT, true);

							irr::gui::IGUIEditBox* editId = (irr::gui::IGUIEditBox*) newObjectDialog->getElementFromId(GUI_EDIT_NEWOBJECT_ID);
							irr::gui::IGUIEditBox* editLoc = (irr::gui::IGUIEditBox*) newObjectDialog->getElementFromId(GUI_EDIT_NEWOBJECT_LOC);
							irr::gui::IGUIEditBox* editModel = (irr::gui::IGUIEditBox*) newObjectDialog->getElementFromId(GUI_EDIT_NEWOBJECT_MODEL);
							irr::gui::IGUIEditBox* editPhys = (irr::gui::IGUIEditBox*) newObjectDialog->getElementFromId(GUI_EDIT_NEWOBJECT_PHYSICS);

							std::string id    = toString(editId->getText());
							std::string loc   = toString(editLoc->getText());
							std::string model = toString(editModel->getText());
							std::string phys  = toString(editPhys->getText());

							Gobject* newObj = new Gobject();

							std::cout << "New Object: " << std::endl;
							std::cout << "id: " << id << std::endl;
							std::cout << "loc: " << loc << std::endl;
							std::cout << "model: " << model << std::endl;
							std::cout << "phys: " << phys << std::endl;

							newObj->id = id;
							newObj->filename = loc;
							newObj->modelFile = model;
							newObj->physicsFile = phys;

							newObj->modelFileExists = false;

							newObj->modified = true;
							newObj->physicsShape.modified = true;

							saveObject(newObj);

							loadedPack->gobjectFiles.push_back(newObj);
							loadedPack->modified = true;
							updateObjectsDialogOnReload();

							openObject(newObj);

							closeDialog(GUI_DIALOG_NEWOBJECT);
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

// SHADER
// =====

class TestShaderCallback : public irr::video::IShaderConstantSetCallBack {
public:
	irr::f32 time;
	TestShaderCallback() {
		time = 0;
	}
	virtual ~TestShaderCallback() {
	}
public:
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {
		services->setVertexShaderConstant("time", &time, 1);

		//services->setVertexShaderConstant();

		irr::s32 texidc = 0;
		services->setPixelShaderConstant("myTexture", &texidc, 1);
	}
};


int main() {

	loadedPack = new Cpack();
	openedObject = new Gobject();

	// Get the preferred driver type
	irr::video::E_DRIVER_TYPE driverType = irr::video::EDT_OPENGL; // driverChoiceConsole(); //
	if(driverType == irr::video::EDT_COUNT) { return 1; }

	// Create the irrlicht device
	AppEventReceiver appEventReceiver;
	irr::SIrrlichtCreationParameters params = irr::SIrrlichtCreationParameters();
	params.DriverType = driverType;
	params.WindowSize = irr::core::dimension2d<irr::u32>(1280, 720);
	params.Bits = 16;
	params.Fullscreen = false;
	params.Stencilbuffer = true;
	params.Vsync = true;
	params.EventReceiver = &appEventReceiver;
	params.AntiAlias = 0;
	device = createDeviceEx(params);
	if(!device) { return 1; }

	device->setResizable(true);

	// Get the driver and manager
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	gui = device->getGUIEnvironment();
	gpuProg = driver->getGPUProgrammingServices();

	// Load shaders?
	TestShaderCallback* ts = new TestShaderCallback();
	shaderMatType = (irr::video::E_MATERIAL_TYPE) gpuProg->addHighLevelShaderMaterialFromFiles(
		"assets/shaders/wave.vert", "vertexMain", irr::video::EVST_VS_1_1,
		"assets/shaders/wave.frag", "pixelMain", irr::video::EPST_PS_1_1,
		ts, irr::video::EMT_SOLID, 0, irr::video::EGSL_DEFAULT);
	ts->drop();

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

		bar->addButton(GUI_BUTTON_DIALOG_OBJECTS, 0, L"Objects", driver->getTexture("assets_editor/objects.png"), 0, false, true);
		bar->addButton(GUI_BUTTON_DIALOG_RESOURCES, 0, L"Resources", driver->getTexture("assets_editor/resources.png"), 0, false, true);
		bar->addButton(GUI_BUTTON_DIALOG_COMPONENTS, 0, L"Components", driver->getTexture("assets_editor/components.png"), 0, false, true);
		bar->addButton(GUI_BUTTON_DIALOG_MODEL, 0, L"Model", driver->getTexture("assets_editor/model.png"), 0, false, true);
		bar->addButton(GUI_BUTTON_DIALOG_PHYSICS, 0, L"Physics", driver->getTexture("assets_editor/physics.png"), 0, false, true);
		bar->addButton(GUI_BUTTON_DIALOG_ANIMATION, 0, L"Animation", driver->getTexture("assets_editor/animation.png"), 0, false, true);
		bar->addButton(GUI_BUTTON_DIALOG_MORPH, 0, L"Morph Targets", driver->getTexture("assets_editor/morph.png"), 0, false, true);
		bar->addButton(GUI_BUTTON_DIALOG_FACIAL, 0, L"Facial Targets", driver->getTexture("assets_editor/facial.png"), 0, false, true);


	}

	// Disable alpha
	for(irr::u32 i = 0; i < irr::gui::EGDC_COUNT; ++ i) {
		irr::video::SColor color = gui->getSkin()->getColor((irr::gui::EGUI_DEFAULT_COLOR) i);
		color.setAlpha(255);
		gui->getSkin()->setColor((irr::gui::EGUI_DEFAULT_COLOR) i, color);
	}

	openAllDialogs();

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


	// Cool ground
	irr::scene::IAnimatedMesh* groundMesh = smgr->getMesh("assets_editor/ground.dae");
	irr::scene::IAnimatedMeshSceneNode* groundNode = smgr->addAnimatedMeshSceneNode(groundMesh);
	irr::video::SMaterial& mat22 = groundNode->getMaterial(0);
	mat22.AmbientColor = irr::video::SColor(255, 255, 255, 255);
	mat22.DiffuseColor = irr::video::SColor(255, 255, 0, 0);

	irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
	irr::scene::ISceneNode* lookAt = smgr->addEmptySceneNode(cam);
	lookAt->setPosition(irr::core::vector3df(0, 0, 1));
	irr::scene::ISceneNode* camUp = smgr->addEmptySceneNode(cam);
	camUp->setPosition(irr::core::vector3df(0, 1, 0));
	irr::scene::ISceneNode* camRight = smgr->addEmptySceneNode(cam);
	camRight->setPosition(irr::core::vector3df(1, 0, 0));
	cam->setPosition(irr::core::vector3df(3, 3, 3));
	cam->setRotation(irr::core::vector3df(45, -135, 0));
	cam->setTarget(lookAt->getAbsolutePosition());

/*
	irr::scene::IAnimatedMesh* arrowMeshY = smgr->addArrowMesh("name", 0xFFFFFFFF, 0xFFFFFFFF, 4, 8, 3);
	smgr->addAnimatedMeshSceneNode(arrowMeshY);
	*/

	//
	irr::scene::ISceneNode* dLightControl = smgr->addEmptySceneNode();
	irr::scene::ILightSceneNode* directionalLight = smgr->addLightSceneNode(dLightControl, irr::core::vector3df(0, 0, 1), irr::video::SColor(1, 1, 1, 1), 10000.0f);
	irr::video::SLight lightData;
	lightData.Type = irr::video::ELT_DIRECTIONAL;
	lightData.Direction = irr::core::vector3df(0, -100, 0);
	lightData.DiffuseColor = irr::video::SColor(255, 100, 100, 100);
	lightData.CastShadows = false;
	directionalLight->setLightData(lightData);
	dLightControl->setRotation(irr::core::vector3df(45, -135, 0));

	smgr->setAmbientLight(irr::video::SColor(255, 155, 155, 155));

	// Initialize tpf calculator
	irr::u32 then = device->getTimer()->getTime();
	irr::u32 frames = 0;
	irr::core::dimension2du screenSize = driver->getScreenSize();
	screenCenter.X = screenSize.Width / 2;
	screenCenter.Y = screenSize.Height / 2;

	irr::f32 lookSpd = 0.1;
	irr::f32 panSpd = 0.01;
	irr::f32 zoomSpd = 0.1;

	irr::core::vector3df inMov;
	irr::core::vector3df upMov;
	irr::core::vector3df rightMov;

	bool panningInitializedByWheel = false;

	// Test
	loadPack("content/standard/content-pack.json");

	// Test
	//irr::scene::SMeshBuffer* buff = reia::loadUsingAssimp(smgr, "content/standard/advanced_mesh/advMesh.dae");
	reia::ComplexMeshData* foobar = reia::loadUsingAssimp(smgr, "assets_editor/worm.dae");
	//reia::ComplexMeshData* foobar = reia::loadUsingAssimp(smgr, "assets_editor/testcube.dae");
	//reia::ComplexMeshData* foobar = reia::loadUsingAssimp(smgr, "example_media/models-nonbsd/MD5/Bob.md5mesh");

	// ultimate foobar
	irr::scene::IAnimatedMesh* armatureDecor = smgr->getMesh("assets_editor/bone.obj");
	armatureDecor->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	armatureDecor->setMaterialFlag(irr::video::EMF_WIREFRAME, true);
 	reia::ComplexMeshSceneNode* baz = reia::addNodeFromMesh(smgr, foobar, font, armatureDecor);
	baz->node->getMaterial(0).Wireframe = true;

	irr::f32 testTime = 4;

	// Main loop
	while(device->run()) {
		if(!device->isWindowActive()) {
			continue;
		}

		// Calculate time per frame in seconds
		const irr::u32 now = device->getTimer()->getTime();
        const irr::f32 tpf = (irr::f32)(now - then) / 1000.f;
        then = now;

		ts->time += tpf;
		testTime += tpf;
		if(testTime > 3.f) {
			testTime -= 3.f;
			std::cout << "pose" << std::endl;
		}
		//reia::poseNode(baz, testTime);

		/*
		for(irr::u32 i = 0; i < buff->Vertices.size(); ++ i) {
			irr::video::S3DVertex& vert = buff->Vertices[i];

			irr::core::vector3df oldLoc = vert.Pos;
			vert.Pos.set(oldLoc.X, oldLoc.Y + tpf, oldLoc.Z + tpf);

		}
		buff->recalculateBoundingBox();
		*/

		// Clear buffers before rendering
		driver->beginScene(true, true, irr::video::SColor(0, 140, 140, 140));


		const irr::core::dimension2du newScreenSize = driver->getScreenSize();
		if(screenSize.Width != newScreenSize.Width || screenSize.Height != newScreenSize.Height) {
			std::cout << "Screen resized." << std::endl;
			screenSize = driver->getScreenSize();
			screenCenter.X = screenSize.Width / 2;
			screenCenter.Y = screenSize.Height / 2;

			irr::f32 aspect = ((irr::f32) screenSize.Width) / ((irr::f32) screenSize.Height);
			std::cout << "Aspect ratio: " << aspect << std::endl;
			if(aspect != 0) {
				cam->setAspectRatio(aspect);
			}
		}

		if(wheelDelta != 0 && moveCamToggle) {
			appState = STATE_PAN;
			panningInitializedByWheel = true;
		}
		else if(panningInitializedByWheel) {
			appState = STATE_EDIT;
			panningInitializedByWheel = false;
		}

		if(appState == STATE_PAN || appState == STATE_LOOK) {
			device->getCursorControl()->setVisible(false);


			// Prevent shaking
			irr::core::position2di mouseMove;

			if(mouseJustClicked) {
				mouseMove.X = 0;
				mouseMove.Y = 0;
				irr::core::vector3df camPos = cam->getAbsolutePosition();
				inMov = lookAt->getAbsolutePosition() - camPos;
				upMov = camUp->getAbsolutePosition() - camPos;
				rightMov = camRight->getAbsolutePosition() - camPos;
				mouseJustClicked = false;
			} else {
				mouseMove.X = mouseLoc.X - screenCenter.X;
				mouseMove.Y = mouseLoc.Y - screenCenter.Y;
			}

			if(appState == STATE_LOOK) {
				irr::core::vector3df rot = cam->getRotation();
				rot.Y += mouseMove.X * lookSpd;
				rot.X += mouseMove.Y * lookSpd;

				if(rot.X > 80) {
					rot.X = 80;
				}
				else if(rot.X < -80) {
					rot.X = -80;
				}

				cam->setRotation(rot);
				cam->setTarget(lookAt->getAbsolutePosition());
				/*
				std::cout << "X" << rot.X << std::endl;
				std::cout << "Y" << rot.Y << std::endl;
				*/
			}
			else if(appState == STATE_PAN) {
				irr::f32 xMov = mouseMove.X * panSpd;
				irr::f32 yMov = mouseMove.Y * panSpd;
				irr::f32 wMov = wheelDelta * zoomSpd;

				irr::core::vector3df camPos = cam->getAbsolutePosition();
				camPos += upMov * yMov;
				camPos += rightMov * -xMov;
				camPos += inMov * wMov;

				cam->setTarget(lookAt->getAbsolutePosition());
				cam->setPosition(camPos);
			}

			device->getCursorControl()->setPosition(screenCenter);

		}
		else {
			device->getCursorControl()->setVisible(true);
		}
		wheelDelta = 0;

		// Draw stuff
		smgr->drawAll();
		driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
		driver->setMaterial(bulletDebugDrawer->material);
		bulletWorld->debugDrawWorld();
		driver->draw3DLine(irr::core::vector3df(0, 0.5, 0), irr::core::vector3df(3, 0.5, 0), irr::video::SColor(255, 255, 0, 0));
		driver->draw3DLine(irr::core::vector3df(0, 0.5, 0), irr::core::vector3df(0, 3.5, 0), irr::video::SColor(255, 0, 255, 0));
		driver->draw3DLine(irr::core::vector3df(0, 0.5, 0), irr::core::vector3df(0, 0.5, 3), irr::video::SColor(255, 0, 0, 255));

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
