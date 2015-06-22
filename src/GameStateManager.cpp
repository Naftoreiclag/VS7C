/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "GameStateManager.h"

#include "GameState.h"
#include "irrlicht.h"

#include <vector>
#include <algorithm>

#define FOR_EACH_STATE std::vector<GameState*>::iterator it = managedStates.begin(); it != managedStates.end(); ++ it


GameStateManager::GameStateManager()
{
}

void GameStateManager::update(const irr::f32 tpf) {
	for(FOR_EACH_STATE) {
		GameState* gs = *it;
		gs->update(tpf);
	}
}

void GameStateManager::render() {
	for(FOR_EACH_STATE) {
		GameState* gs = *it;
		gs->render();
	}
}

GameStateManager::~GameStateManager() {
	detachAll();
}

void GameStateManager::attachState(GameState& state) {
	managedStates.push_back(&state);
	state.init();
	state.grab();
}

void GameStateManager::detachAll() {
	// For each state
	for(FOR_EACH_STATE) {
		// Properly manage lifecycle
		GameState* gs = *it;
		gs->cleanup();
		gs->drop();
	}

	// Remove all
	managedStates.clear();
}

void GameStateManager::detachState(GameState& state) {

	// For each state
	for(FOR_EACH_STATE) {
		// Properly manage lifecycle
		GameState* gs = *it;
		if(gs == &state) {
			gs->cleanup();
			gs->drop();
		}
	}

	// Remove all the dropped states
	managedStates.erase(std::remove(managedStates.begin(), managedStates.end(), &state), managedStates.end());


}
