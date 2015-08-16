#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "irrlicht.h"

namespace skm {

class InputManager : public irr::IEventReceiver {
public:
    virtual bool OnEvent(const irr::SEvent& event);
    bool isKeyDown(irr::EKEY_CODE keyCode) const;

    InputManager();
private:
    bool isKeyDownMap[irr::KEY_KEY_CODES_COUNT];
};

}

#endif // INPUTMANAGER_H
