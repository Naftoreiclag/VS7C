/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "PlayerComponent.h"

PlayerComponent::PlayerComponent(const InputManager* const inputMgr)
: artemis::Component(),
inputMgr(inputMgr) {
	//ctor
}

PlayerComponent::~PlayerComponent() {
	//dtor
}



PlayerSystem::PlayerSystem() {
	addComponentType<PhysicsComponent>();
	addComponentType<CharacterPhysicsComponent>();
	addComponentType<PlayerComponent>();
}

void PlayerSystem::initialize() {
	physicsMapper.init(*world);
	characterPhysicsMapper.init(*world);
	playerMapper.init(*world);
}

void PlayerSystem::processEntity(artemis::Entity& e) {
	phys = physicsMapper.get(e);
	charPhys = characterPhysicsMapper.get(e);
	player = playerMapper.get(e);



	player->inputMgr->notifyMe(player, this);
}

void PlayerSystem::keyPressed(irr::EKEY_CODE key) {


}
void PlayerSystem::keyDown(irr::EKEY_CODE key) {


}

