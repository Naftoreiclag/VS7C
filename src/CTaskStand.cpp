/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTaskStand.h"

#include "CTConditionSitting.h"
#include <iostream>

CharacterTaskStand::CharacterTaskStand() {
}

CharacterTaskStand::~CharacterTaskStand() {
}

std::vector<CharacterTaskCondition*> CharacterTaskStand::getPrerequisites() const {
	std::vector<CharacterTaskCondition*> retVal;
	CharacterTaskCondition* condition = new CTConditionSitting(true);
	retVal.push_back(condition);
	return retVal;
}

CharacterTaskStand* CharacterTaskStand::newWhichFulfills(const CharacterTaskCondition* condition) const {
	return new CharacterTaskStand();
}

bool CharacterTaskStand::fulfills(const CharacterTaskCondition* condition) const {
	const CTConditionSitting* sitting = static_cast<const CTConditionSitting*>(condition);
	if(sitting) {
		return sitting->sitting == false;
	}

	return false;
}

bool CharacterTaskStand::process(CharacterState& state, irr::f32 tpf) {
	std::cout << "you stand up" << std::endl;
    state.body->sitting = false;
    return true;
}

bool CharacterTaskStand::isCompleted(const CharacterState& state) const {
    return !state.body->sitting;
}
