/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTaskStand.h"

#include <iostream>

CharacterTaskStand::CharacterTaskStand()
{
	//ctor
}

CharacterTaskStand::~CharacterTaskStand()
{
	//dtor
}

CharacterTaskStand* CharacterTaskStand::clone() const {
	return new CharacterTaskStand();
}


std::vector<CharacterTaskCondition> CharacterTaskStand::getPrerequisites() const {
	std::vector<CharacterTaskCondition> retVal;
	CharacterTaskCondition condition;
	condition.sitting = true;
	retVal.push_back(condition);
	return retVal;
}

std::vector<CharacterTaskCondition> CharacterTaskStand::getEffects() const {
	std::vector<CharacterTaskCondition> retVal;
	CharacterTaskCondition condition;
	condition.sitting = false;
	retVal.push_back(condition);
	return retVal;
}

bool CharacterTaskStand::fulfills(const CharacterTaskCondition& condition) const {
	return condition.sitting == false;
}

void CharacterTaskStand::process(CharacterState& state, irr::f32 tpf) {
	std::cout << "you stand up" << std::endl;
    state.body->sitting = false;
}

bool CharacterTaskStand::isCompleted(const CharacterState& state) const {
    return !state.body->sitting;
}
