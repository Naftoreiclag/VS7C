/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterComponent.h"

CharacterComponent::CharacterComponent()
{
	//ctor
}

CharacterComponent* CharacterComponent::clone() const {
	return new CharacterComponent();
}

CharacterComponent::~CharacterComponent()
{
}

