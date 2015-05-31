#ifndef InputManager_H
#define InputManager_H

#include "irrlicht.h"

class InputManager : public irr::IEventReceiver
{
public:
	InputManager();
	virtual bool OnEvent(const irr::SEvent& event);
	bool isKeyDown(irr::EKEY_CODE keyCode) const;
private:
	bool isKeyDownMap[irr::KEY_KEY_CODES_COUNT];
};

#endif
