/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef RENDERTESTGAMESTATE_H
#define RENDERTESTGAMESTATE_H

#include "GameState.h"

class RenderTestGameState : public GameState {
public:
	RenderTestGameState(irr::IrrlichtDevice *irrlicht);
	virtual ~RenderTestGameState();

	virtual void init();
	virtual void cleanup();

	virtual void pause();
	virtual void resume();

	virtual void update(const irr::f32 tpf);
	virtual void render();
private:
	irr::video::IVideoDriver* driver;
	irr::scene::ISceneManager* smgr;
	irr::scene::ICameraSceneNode* cam;
	irr::IrrlichtDevice* device;
};

#endif // RENDERTESTGAMESTATE_H
