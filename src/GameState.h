#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "irrlicht.h"

class GameState : public irr::IReferenceCounted {
public:
	GameState(irr::IrrlichtDevice* irrlicht);

	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void update(const irr::f32 tpf) = 0;
	virtual void render() = 0;
};

#endif // GAMESTATE_H
