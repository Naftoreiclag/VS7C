/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "InputManager.h"

InputManager::InputManager() {
	for(irr::u32 i = 0; i < irr::KEY_KEY_CODES_COUNT; ++ i) {
		isKeyDownMap[i] = false;
	}

	for(MCODE i = 0; i < MCODE_COUNT; ++ i) {
		isMouseDownMap[i] = false;
	}
}

void InputManager::notifyMe(InputReceiver* receiver, InputReceiver* callMeInstead) const {
	// If the receiver was never notified before, initialize all the variables
	if(!receiver->lastDataReceieved.notifiedBefore) {
		// Ensures no analog data is sent
		receiver->lastDataReceieved.toldMouseLoc = mouseLoc;

		// All keys are up by default
		for(irr::u32 i = 0; i < irr::KEY_KEY_CODES_COUNT; ++ i) {
			receiver->lastDataReceieved.toldKeyPressed[i] = false;
		}
		for(MCODE i = 0; i < MCODE_COUNT; ++ i) {
			receiver->lastDataReceieved.toldMousePressed[i] = false;
		}

		// We are about to notify it, but set this boolean in advance
		receiver->lastDataReceieved.notifiedBefore = true;
	}

	// Mouse movement
	if(receiver->lastDataReceieved.toldMouseLoc != mouseLoc) {
		callMeInstead->mouseMove(mouseLoc);
		receiver->lastDataReceieved.toldMouseLoc = mouseLoc;
	}

    // Keys
    for(irr::u32 i = 0; i < irr::KEY_KEY_CODES_COUNT; ++ i) {
        // If the key is currently down
        if(isKeyDownMap[i]) {
			// If the receiver was not already notified that it is down
			if(!receiver->lastDataReceieved.toldKeyPressed[i]) {
				// Tell the receiver that the key has switched from being up to being down
				callMeInstead->keyPressed((irr::EKEY_CODE) i);
				receiver->lastDataReceieved.toldKeyPressed[i] = true;
			}
			// Send the "key is down" thing
			callMeInstead->keyDown((irr::EKEY_CODE) i);
        }

        // The key is currently up
        else {
			// If the receiver was not updated that the key is now up
			if(receiver->lastDataReceieved.toldKeyPressed[i]) {
				// Tell the receiver that the key has switched from being down to being up
                callMeInstead->keyReleased((irr::EKEY_CODE) i);
				receiver->lastDataReceieved.toldKeyPressed[i] = false;
			}
        }
    }

    // Mice, basically the same thing
    for(MCODE i = 0; i < MCODE_COUNT; ++ i) {
		// If the button is currently down
        if(isMouseDownMap[i]) {
			// If the receiver was not already notified that it is down
			if(!receiver->lastDataReceieved.toldMousePressed[i]) {
				// Tell the receiver that the button has switched from being up to being down
				callMeInstead->mousePressed(i);
				receiver->lastDataReceieved.toldMousePressed[i] = true;
			}
			// Send the "button is down" thing
			callMeInstead->mouseDown(i);
        }

        // The button is currently up
        else {
			// If the receiver was not updated that the button is now up
			if(receiver->lastDataReceieved.toldMousePressed[i]) {
				// Tell the receiver that the button has switched from being down to being up
                callMeInstead->mouseReleased(i);
				receiver->lastDataReceieved.toldMousePressed[i] = false;
			}
        }
    }
}

void InputManager::notifyMe(InputReceiver* receiver) const {
	notifyMe(receiver, receiver);
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
				isMouseDownMap[MCODE_LEFT] = true;
			break;
			case irr::EMIE_LMOUSE_LEFT_UP:
				isMouseDownMap[MCODE_LEFT] = false;
			break;
			case irr::EMIE_RMOUSE_PRESSED_DOWN:
				isMouseDownMap[MCODE_RIGHT] = true;
			break;
			case irr::EMIE_RMOUSE_LEFT_UP:
				isMouseDownMap[MCODE_RIGHT] = false;
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

bool InputManager::isMouseDown(MCODE buttonCode) const {
	return isMouseDownMap[buttonCode];
}

irr::core::position2di InputManager::getMouseLoc() const {
	return mouseLoc;
}


InputReceiver::~InputReceiver() {}
void InputReceiver::keyPressed(irr::EKEY_CODE key) {}
void InputReceiver::keyDown(irr::EKEY_CODE key) {}
void InputReceiver::keyReleased(irr::EKEY_CODE key) {}

void InputReceiver::mousePressed(irr::s8 button) {}
void InputReceiver::mouseDown(irr::s8 button) {}
void InputReceiver::mouseReleased(irr::s8 button) {}
void InputReceiver::mouseMove(irr::core::position2di mouseLoc) {}
