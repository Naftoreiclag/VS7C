/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterPerformerComponent.h"

CharacterPerformerComponent::CharacterPerformerComponent()
: currentObjective(0),
currentAction(0)
{
	//ctor
}
CharacterPerformerComponent* CharacterPerformerComponent::clone() const {
	return new CharacterPerformerComponent();
}

CharacterPerformerComponent::~CharacterPerformerComponent()
{
	//dtor
}