/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

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

	GUI_DIALOG_RESOURCES,

    GUI_TOGGLE_RESOURCES,
    GUI_TOGGLE_COMPONENTS,
    GUI_TOGGLE_MODEL,
    GUI_TOGGLE_PHYSICS,
    GUI_TOGGLE_ANIMATION,
};

inline irr::core::rect<irr::s32> GuiRect(irr::s32 x, irr::s32 y, irr::s32 width, irr::s32 height) {
	return irr::core::rect<irr::s32>(x, y, x + width, y + height);
}

void closeAllDialogs() {

}

void showResourcesDialog() {
	// Remove dialog if already open
	irr::gui::IGUIElement* elem = gui->getRootGUIElement()
		->getElementFromId(GUI_DIALOG_RESOURCES, true);
	if(elem) {
		elem->remove();
	}

	// Create dialog
	irr::gui::IGUIWindow* dialog = gui->addWindow(GuiRect(10, 60, 150, 300), false, L"Resources", 0, GUI_DIALOG_RESOURCES);

	// Add tree view


}

class AppEventReceiver : public irr::IEventReceiver {
public:
	virtual bool OnEvent(const irr::SEvent& event) {

		if(event.EventType == irr::EET_GUI_EVENT) {
			irr::s32 id = event.GUIEvent.Caller->getID();

			switch(event.GUIEvent.EventType) {
			case irr::gui::EGET_BUTTON_CLICKED:
				switch(id) {
				case GUI_TOGGLE_RESOURCES:
					showResourcesDialog();
				}
			break;
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
		submenu->addItem(L"New");
		submenu->addItem(L"Open");
		submenu->addItem(L"Save");
		submenu->addItem(L"Save as...");
	}

	{
		irr::gui::IGUIToolBar* bar = gui->addToolBar();

		irr::video::ITexture* image = driver->getTexture("assets/A.png");
		bar->addButton(GUI_TOGGLE_RESOURCES, 0, L"Resources", image, 0, false, true);
		bar->addButton(GUI_TOGGLE_COMPONENTS, 0, L"Components", image, 0, false, true);
		bar->addButton(GUI_TOGGLE_MODEL, 0, L"Model", image, 0, false, true);
		bar->addButton(GUI_TOGGLE_PHYSICS, 0, L"Physics", image, 0, false, true);
		bar->addButton(GUI_TOGGLE_ANIMATION, 0, L"Animation", image, 0, false, true);


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
