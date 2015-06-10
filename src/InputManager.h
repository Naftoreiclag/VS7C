#ifndef InputManager_H
#define InputManager_H

#include "irrlicht.h"

class InputManager : public irr::IEventReceiver
{
public:
	InputManager();
	virtual bool OnEvent(const irr::SEvent& event);
	bool isKeyDown(irr::EKEY_CODE keyCode) const;
	bool isLMouseDown() const;
	bool isRMouseDown() const;
	irr::core::position2di getMouseLoc() const;
private:
	bool isKeyDownMap[irr::KEY_KEY_CODES_COUNT];
	bool isLMouseDownBool;
	bool isRMouseDownBool;
	irr::core::position2di mouseLoc;
};

#endif
