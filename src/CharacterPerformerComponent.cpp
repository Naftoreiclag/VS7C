/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterPerformerComponent.h"

CharacterPerformerComponent::CharacterPerformerComponent()
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

CharacterPerformerComponent::TaskLayer::TaskLayer(CharacterTask* const task)
: task(task) {
}
