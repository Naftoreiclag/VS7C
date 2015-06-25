/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include "irrlicht.h"
#include <vector>

class GameState;

class GameStateManager {
public:
	GameStateManager();
	virtual ~GameStateManager();

	void attachState(GameState* state);
	void detachState(GameState* state);
	void detachAll();

	void update(const irr::f32 tpf);
	void render();
private:
	std::vector<GameState*> managedStates;
};

#endif // GAMESTATEMANAGER_H
