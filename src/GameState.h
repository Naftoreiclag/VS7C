#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "irrlicht.h"

using namespace irr;

class GameState
{
public:
	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void update(const u32 tpf) = 0;

protected:
	GameState();
};

#endif // GAMESTATE_H
