#ifndef OVERWORLDGAMESTATE_H
#define OVERWORLDGAMESTATE_H

#include "irrlicht.h"
using namespace irr;

#include "GameState.h"

class ChunkNode;

class OverworldGameState : public GameState
{
public:
	OverworldGameState(IrrlichtDevice *irrlicht);

	virtual void init();
	virtual void cleanup();

	virtual void pause();
	virtual void resume();

	virtual void update(const f32 tpf);
	virtual void render();
private:
	video::IVideoDriver* driver;
	scene::ISceneManager* smgr;

	ChunkNode* chunkNode;
};

#endif // OVERWORLDGAMESTATE_H
