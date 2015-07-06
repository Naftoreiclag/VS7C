/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterBodyComponent.h"

#include "json/json.h"

CharacterBodyComponent::CharacterBodyComponent() {

}

CharacterBodyComponent* CharacterBodyComponent::clone() const {
	return new CharacterBodyComponent();
}

CharacterBodyComponent::~CharacterBodyComponent() {

}

nres::ComponentData* CharacterBodySerializer::read(const Json::Value& data) {
	CharacterBodyComponent* comp = new CharacterBodyComponent();

	comp->sitting = data["sitting"].asBool();

	return comp;
}

Json::Value CharacterBodySerializer::write(const nres::ComponentData* compData) {
	const CharacterBodyComponent* comp = static_cast<const CharacterBodyComponent*>(compData);
	Json::Value data;

	data["sitting"] = comp->sitting;

	return data;
}
