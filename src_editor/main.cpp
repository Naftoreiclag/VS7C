/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include <iostream>
#include "irrlicht.h"
#include "driverChoice.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

irr::video::IVideoDriver* driver;
irr::scene::ISceneManager* smgr;
irr::gui::IGUIEnvironment* gui;

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
};

inline irr::core::rect<irr::s32> GuiBox(irr::s32 x, irr::s32 y, irr::s32 width, irr::s32 height) {
	return irr::core::rect<irr::s32>(x, y, x + width, y + height);
}
inline irr::core::rect<irr::s32> GuiRect(irr::s32 x1, irr::s32 y1, irr::s32 x2, irr::s32 y2) {
	return irr::core::rect<irr::s32>(x1, y1, x2, y2);
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

void showResourcesDialog() {
	closeDialog(GUI_DIALOG_RESOURCES);

	// Create dialog
	irr::gui::IGUIWindow* dialog = gui->addWindow(GuiBox(10, 60, 200, 300), false, L"Resources", 0, GUI_DIALOG_RESOURCES);

	// Add tree view
	irr::gui::IGUITreeView* tree = gui->addTreeView(GuiRect(5, 25, 195, 295), dialog, GUI_TREE_RESOURCES, true, true, true);

	irr::gui::IGUITreeViewNode* root = tree->getRoot();

	root->addChildBack(L"Test");

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
		driver->beginScene(true, true, irr::video::SColor(0, 255, 255, 255));

		// Draw stuff
		smgr->drawAll();
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

	return 0;
}
