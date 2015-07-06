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

bool CharacterPerformerComponent::isBusy() const {
	if(currentObjective.conditionToFulfill != 0) {
		return true;
	}
	if(currentObjective.taskToPerform != 0) {
		return true;
	}
	if(!taskLayers.empty()) {
		return true;
	}
	if(waitingFor != 0) {
		return true;
	}
	return false;
}

CharacterPerformerComponent::TaskLayer::TaskLayer(CharacterTask* const task)
: task(task) {
}

nres::ComponentData* CharacterPerformerSerializer::read(const Json::Value& data) {
	CharacterPerformerComponent* comp = new CharacterPerformerComponent();

	return comp;
}

Json::Value CharacterPerformerSerializer::write(const nres::ComponentData* compData) {
	const CharacterPerformerComponent* comp = static_cast<const CharacterPerformerComponent*>(compData);
	Json::Value data;

	return data;
}
