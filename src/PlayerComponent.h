/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "NREntitySystem.h"
#include "ComponentIDs.h"

#include "CharacterPhysicsComponent.h"
#include "PhysicsComponent.h"

#include "InputManager.h"

/*
Marks an entity as player-controlled
*/
/*
class PlayerComponent : public nres::ComponentData, public InputReceiver {
public:
	const InputManager* const inputMgr;

	PlayerComponent(const InputManager* const inputMgr);
	~PlayerComponent();
};

class PlayerSystem : public nres::System, public InputReceiver {
private:
	nres::ComponentID accessedComponents[3] = {compIDs::CID_PHYSICS, compIDs::CID_CHARPHYSICS, compIDs::CID_PLAYER};
public:
	PlayerSystem();
	virtual void initialize();
	virtual void processEntity(nres::Entity& e);

private:
	PhysicsComponent* phys;
	CharacterPhysicsComponent* charPhys;
	PlayerComponent* player;

public:
	virtual void keyPressed(irr::EKEY_CODE key);
	virtual void keyDown(irr::EKEY_CODE key);
};
*/

#endif // PLAYERCOMPONENT_H
