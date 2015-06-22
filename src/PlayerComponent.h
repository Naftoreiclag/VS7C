/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "Artemis/Artemis.h"

#include "CharacterPhysicsComponent.h"
#include "PhysicsComponent.h"

#include "InputManager.h"

/*
Marks an entity as player-controlled
*/
class PlayerComponent : public artemis::Component, public InputReceiver {
public:
	const InputManager* const inputMgr;

	PlayerComponent(const InputManager* const inputMgr);
	~PlayerComponent();
};

class PlayerSystem : public artemis::EntityProcessingSystem, public InputReceiver {
private:
	artemis::ComponentMapper<PhysicsComponent> physicsMapper;
	artemis::ComponentMapper<CharacterPhysicsComponent> characterPhysicsMapper;
	artemis::ComponentMapper<PlayerComponent> playerMapper;

public:
	PlayerSystem();
	virtual void initialize();
	virtual void processEntity(artemis::Entity& e);

private:
	PhysicsComponent* phys;
	CharacterPhysicsComponent* charPhys;
	PlayerComponent* player;

public:
	virtual void keyPressed(irr::EKEY_CODE key);
	virtual void keyDown(irr::EKEY_CODE key);
};

#endif // PLAYERCOMPONENT_H
