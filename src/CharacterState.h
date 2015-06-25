/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERSTATE_H
#define CHARACTERSTATE_H

#include "CharacterBodyComponent.h"

class CharacterState{
public:
	CharacterState(CharacterBodyComponent* body);
	virtual ~CharacterState();
	CharacterBodyComponent* body;
};

#endif // CHARACTERSTATE_H
