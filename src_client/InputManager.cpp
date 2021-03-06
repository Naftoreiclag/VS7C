#include "InputManager.h"

namespace skm {

bool InputManager::OnEvent(const irr::SEvent& event) {
	// Keys
	if(event.EventType == irr::EET_KEY_INPUT_EVENT) {
		isKeyDownMap[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}

    return false;
}

bool InputManager::isKeyDown(irr::EKEY_CODE keyCode) const {
	return isKeyDownMap[keyCode];
}

InputManager::InputManager() {
	for(irr::u32 i = 0; i < irr::KEY_KEY_CODES_COUNT; ++ i) {
		isKeyDownMap[i] = false;
	}
}

}
