#ifndef InputManager_H
#define InputManager_H

#include "irrlicht.h"

class InputReceiver;

typedef unsigned int MCODE;

class InputManager : public irr::IEventReceiver {
public:

	static const MCODE MCODE_LEFT = 0;
	static const MCODE MCODE_RIGHT = 1;
	static const MCODE MCODE_MIDDLE = 2;

	static const MCODE MCODE_COUNT = 3;

	InputManager();
	virtual bool OnEvent(const irr::SEvent& event);

	void notifyMe(InputReceiver* receiver);
	bool isKeyDown(irr::EKEY_CODE keyCode) const;
	bool isMouseDown(MCODE buttonCode) const;
	irr::core::position2di getMouseLoc() const;
private:
	bool isKeyDownMap[irr::KEY_KEY_CODES_COUNT];
	bool isMouseDownMap[MCODE_COUNT];
	irr::core::position2di mouseLoc;
};

class InputReceiver {
public:
	struct PreviousReception {
		bool notifiedBefore = false;
		bool toldKeyPressed[irr::KEY_KEY_CODES_COUNT];
		bool toldMousePressed[InputManager::MCODE_COUNT];
		irr::core::position2di toldMouseLoc;
	};

	PreviousReception lastDataReceieved;

	virtual ~InputReceiver();
	virtual void keyPressed(irr::EKEY_CODE key);
	virtual void keyDown(irr::EKEY_CODE key);
	virtual void keyReleased(irr::EKEY_CODE key);

	virtual void mousePressed(irr::s8 button);
	virtual void mouseDown(irr::s8 button);
	virtual void mouseReleased(irr::s8 button);
	virtual void mouseMove(irr::core::position2di mouseLoc);
};

#endif
