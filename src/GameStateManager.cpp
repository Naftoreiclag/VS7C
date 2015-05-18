#include "GameStateManager.h"

#include "GameState.h"
#include "irrlicht.h"
#include <vector>


GameStateManager::GameStateManager() {

}

GameStateManager::~GameStateManager() {
	for(std::vector<GameState*>::iterator it = managedStates.begin(); it != managedStates.end(); ++ it) {

		GameState* gs = *it;

		if(gs->isPaused()) {
			gs->resume();
		}

		gs->cleanup();
		gs->drop();
	}
}
