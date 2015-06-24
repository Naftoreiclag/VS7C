/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterIdentityComponent.h"

CharacterIdentityComponent::CharacterIdentityComponent()
{
	//ctor
}
CharacterIdentityComponent* CharacterIdentityComponent::clone() const {
	return new CharacterIdentityComponent();
}

CharacterIdentityComponent::~CharacterIdentityComponent()
{
	//dtor
}
