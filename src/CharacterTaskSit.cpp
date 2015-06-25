/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterTaskSit.h"

#include <iostream>

CharacterTaskSit::CharacterTaskSit()
{
	//ctor
}

CharacterTaskSit::~CharacterTaskSit()
{
	//dtor
}

CharacterTaskSit* CharacterTaskSit::clone() const {
	return new CharacterTaskSit();
}


std::vector<CharacterTaskCondition> CharacterTaskSit::getPrerequisites() const {
	std::vector<CharacterTaskCondition> retVal;
	CharacterTaskCondition condition;
	condition.sitting = false;
	retVal.push_back(condition);
	return retVal;
}

std::vector<CharacterTaskCondition> CharacterTaskSit::getEffects() const {
	std::vector<CharacterTaskCondition> retVal;
	CharacterTaskCondition condition;
	condition.sitting = true;
	retVal.push_back(condition);
	return retVal;
}

bool CharacterTaskSit::fulfills(const CharacterTaskCondition& condition) const {
	return condition.sitting == true;
}

void CharacterTaskSit::process(CharacterState& state, irr::f32 tpf) {
	std::cout << "you sit down" << std::endl;
    return state.body->sitting = true;
}

bool CharacterTaskSit::isCompleted(const CharacterState& state) const {
    return state.body->sitting;
}
