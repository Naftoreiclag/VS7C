#include "InputManager.h"

InputManager::InputManager() {
	for(irr::u32 i = 0; i < irr::KEY_KEY_CODES_COUNT; ++ i) {
		isKeyDownMap[i] = false;
	}

	isLMouseDownBool = false;
	isRMouseDownBool = false;
}

bool InputManager::OnEvent(const irr::SEvent& event) {
	// Keys
	if(event.EventType == irr::EET_KEY_INPUT_EVENT) {
		isKeyDownMap[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}

	// Mice
	if(event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
		switch(event.MouseInput.Event) {
			case irr::EMIE_LMOUSE_PRESSED_DOWN:
				isLMouseDownBool = true;
			break;
			case irr::EMIE_LMOUSE_LEFT_UP:
				isLMouseDownBool = false;
			break;
			case irr::EMIE_RMOUSE_PRESSED_DOWN:
				isRMouseDownBool = true;
			break;
			case irr::EMIE_RMOUSE_LEFT_UP:
				isRMouseDownBool = false;
			break;
			case irr::EMIE_MOUSE_MOVED:
				mouseLoc.X = event.MouseInput.X;
				mouseLoc.Y = event.MouseInput.Y;
				break;
			default:
				break;
		}
	}

	return false;
}

bool InputManager::isKeyDown(irr::EKEY_CODE keyCode) const {
	return isKeyDownMap[keyCode];
}

bool InputManager::isLMouseDown() const {
	return isLMouseDownBool;
}

bool InputManager::isRMouseDown() const {
	return isRMouseDownBool;
}

irr::core::position2di InputManager::getMouseLoc() const {
	return mouseLoc;
}
