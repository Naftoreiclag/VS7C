/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "irrlicht.h"

class GameState {
public:
	GameState(irr::IrrlichtDevice* irrlicht);
	virtual ~GameState();

	virtual void init() = 0;
	virtual void cleanup() = 0;

	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void update(const irr::f32 tpf) = 0;
	virtual void render() = 0;
};

#endif // GAMESTATE_H
