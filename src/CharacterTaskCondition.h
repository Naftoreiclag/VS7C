/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERTASKCONDITION_H
#define CHARACTERTASKCONDITION_H

#include "CharacterBodyComponent.h"

/*
Atomic condition, such as sitting down, standing, located somewhere, etc...

*/

class CharacterTaskCondition {
public:
	struct CharacterState {
		CharacterState(CharacterBodyComponent* body)
		:body(body) {
		}
		CharacterBodyComponent* body;
	};
public:
	CharacterTaskCondition();
	virtual ~CharacterTaskCondition();
	bool isFulfilled(CharacterState state);
};

#endif // CHARACTERTASKCONDITION_H
