#ifndef OVERWORLDGAMESTATE_H
#define OVERWORLDGAMESTATE_H

#include "irrlicht.h"
using namespace irr;

#include "GameState.h"

class OverworldGameState : public GameState
{
public:
	virtual void init();
	virtual void cleanup();

	virtual void pause();
	virtual void resume();

	virtual void update(const f32 tpf);
	virtual void render();
private:
	video::IVideoDriver* driver;
	scene::ISceneManager* smgr;
protected:
	OverworldGameState(IrrlichtDevice *irrlicht);
};

#endif // OVERWORLDGAMESTATE_H
