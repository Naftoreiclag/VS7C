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
#include <iostream>


GameStateManager::GameStateManager()
{
}

void GameStateManager::update(const irr::f32 tpf) {
	for(std::vector<GameState*>::iterator it = managedStates.begin(); it != managedStates.end(); ++ it) {
		GameState* gs = *it;
		gs->update(tpf);
	}
}

void GameStateManager::render() {
	for(std::vector<GameState*>::iterator it = managedStates.begin(); it != managedStates.end(); ++ it) {
		GameState* gs = *it;
		gs->render();
	}
}

GameStateManager::~GameStateManager() {
	detachAll();
}

void GameStateManager::attachState(GameState* state) {
	managedStates.push_back(state);
	state->init();
	state->grab();
}

void GameStateManager::detachAll() {

	std::cout << "detaching all game states" << std::endl;

	// For each state
	for(std::vector<GameState*>::iterator it = managedStates.begin(); it != managedStates.end(); ++ it) {
		std::cout << "dropping..." << std::endl;
		// Properly manage lifecycle
		GameState* gs = *it;
		gs->cleanup();
		gs->drop();
		std::cout << "dropped." << std::endl;
	}

	// Remove all
	managedStates.clear();
	std::cout << "completed detaching all game states" << std::endl;
}

void GameStateManager::detachState(GameState* state) {

	// For each state
	for(std::vector<GameState*>::iterator it = managedStates.begin(); it != managedStates.end(); ++ it) {
		// Properly manage lifecycle
		GameState* gs = *it;
		if(gs == state) {
			gs->cleanup();
			gs->drop();
		}
	}

	// Remove all the dropped states
	managedStates.erase(std::remove(managedStates.begin(), managedStates.end(), state), managedStates.end());


}
