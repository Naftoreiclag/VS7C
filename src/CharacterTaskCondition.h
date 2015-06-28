/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERTASKCONDITION_H
#define CHARACTERTASKCONDITION_H

#include "CharacterBodyComponent.h"
#include "CharacterPhysicsComponent.h"
#include "PhysicsComponent.h"

/*
Specifies a particular condition that a characterstate might be in.
Atomic condition, such as sitting down, standing, located somewhere, etc...
*/

struct CharacterState {
	CharacterState(CharacterBodyComponent* body, CharacterPhysicsComponent* charPhys, PhysicsComponent* phys)
	:body(body),
	charPhys(charPhys),
	phys(phys) {
	}
	CharacterBodyComponent* body;
	CharacterPhysicsComponent* charPhys;
	PhysicsComponent* phys;
};

class CharacterTaskCondition {
public:
	CharacterTaskCondition();
	virtual ~CharacterTaskCondition();
	virtual bool isFulfilled(const CharacterState& state) const = 0;
	virtual bool isPossible(const CharacterState& state);
};

#endif // CHARACTERTASKCONDITION_H
