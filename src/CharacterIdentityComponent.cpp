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

nres::ComponentData* CharacterIdentitySerializer::read(const Json::Value& data) {
	CharacterIdentityComponent* comp = new CharacterIdentityComponent();

	return comp;
}

Json::Value CharacterIdentitySerializer::write(const nres::ComponentData* compData) {
	const CharacterIdentityComponent* comp = static_cast<const CharacterIdentityComponent*>(compData);
	Json::Value data;

	return data;
}

