/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERTASKCONDITION_H
#define CHARACTERTASKCONDITION_H

#include "irrlicht.h"

#include "nresEntity.h"

#include "CharacterPerformerComponent.h"
#include "CharacterBodyComponent.h"
#include "CharacterPhysicsComponent.h"
#include "PhysicsComponent.h"

/*
Specifies a particular condition that a characterstate might be in.
Atomic condition, such as sitting down, standing, located somewhere, etc...
*/

struct CharacterState {
	CharacterState(CharacterPerformerComponent* perf, CharacterBodyComponent* body, CharacterPhysicsComponent* charPhys, PhysicsComponent* phys);

	CharacterState(nres::Entity& entity);

	CharacterPerformerComponent* perf;
	CharacterBodyComponent* body;
	CharacterPhysicsComponent* charPhys;
	PhysicsComponent* phys;

	bool valid;
};

class CharacterTaskCondition {
public:
	CharacterTaskCondition();
	virtual ~CharacterTaskCondition();
	virtual bool isFulfilled(const CharacterState& state) const = 0;
	virtual bool isValid(const CharacterState& state) const;

	/*
	If true, the performer attempts to fulfill this condition
	only once before fulfilling any sibling conditions.
	If ever this condition becomes unfulfilled, then the
	whole task is counted as interrupted.
	*/
	bool continuous = false;

	/*
	If the performer cannot fulfill this condition for whatever reason,
	rather than instantly cancelling the objective, the performer can
	wait for some amount of seconds.
	*/
	irr::f32 waitTimeout = 0;
};

#endif // CHARACTERTASKCONDITION_H
