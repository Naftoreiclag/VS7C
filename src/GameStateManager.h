#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include "irrlicht.h"
#include <vector>

class GameState;

class GameStateManager {
public:
	GameStateManager();
	virtual ~GameStateManager();

	void attachState(GameState& state);
	void detachState(GameState& state);
	void detachAll();

	void update(const irr::f32 tpf);
	void render();
private:
	std::vector<GameState*> managedStates;
};

#endif // GAMESTATEMANAGER_H
