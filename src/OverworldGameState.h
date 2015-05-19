#ifndef OVERWORLDGAMESTATE_H
#define OVERWORLDGAMESTATE_H

#include "GameState.h"

class OverworldGameState : public GameState
{
	public:
		virtual void init();
		virtual void cleanup();

		virtual void pause();
		virtual void resume();

		virtual void update(const irr::f32 tpf);
		virtual void render();
	protected:
		OverworldGameState();
};

#endif // OVERWORLDGAMESTATE_H
