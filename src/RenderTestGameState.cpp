/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "RenderTestGameState.h"

RenderTestGameState::RenderTestGameState(irr::IrrlichtDevice *irrlicht)
: GameState::GameState(irrlicht) {
	device = irrlicht;
	driver = irrlicht->getVideoDriver();
	smgr = irrlicht->getSceneManager();
}

RenderTestGameState::~RenderTestGameState()
{
	//dtor
}


void RenderTestGameState::init() {
	cam = smgr->addCameraSceneNodeMaya();
}

void RenderTestGameState::cleanup() {

}

void RenderTestGameState::pause() {

}

void RenderTestGameState::resume() {

}

void RenderTestGameState::update(const irr::f32 tpf) {

}

void RenderTestGameState::render() {
	smgr->drawAll();
}
