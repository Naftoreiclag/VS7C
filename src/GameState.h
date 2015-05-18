#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "irrlicht.h"

class GameState : public irr::IReferenceCounted
{
public:
	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() = 0;
	virtual bool isPaused() = 0;
	virtual void resume() = 0;

	virtual void update(const irr::f32 tpf) = 0;

protected:
	GameState();
};

#endif // GAMESTATE_H
