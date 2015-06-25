/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTaskConversation.h"

#include <iostream>

CTaskConversation::CTaskConversation()
{
	//ctor
}

CTaskConversation::~CTaskConversation()
{
	//dtor
}

CTaskConversation* CTaskConversation::clone() const {
	return new CTaskConversation();
}


std::vector<CharacterTaskCondition> CTaskConversation::getPrerequisites() const {
	std::vector<CharacterTaskCondition> retVal;
	CharacterTaskCondition condition;
	condition.sitting = false;
	retVal.push_back(condition);
	return retVal;
}

std::vector<CharacterTaskCondition> CTaskConversation::getEffects() const {
	std::vector<CharacterTaskCondition> retVal;
	CharacterTaskCondition condition;
	condition.sitting = true;
	retVal.push_back(condition);
	return retVal;
}

bool CTaskConversation::fulfills(const CharacterTaskCondition& condition) const {
	return condition.sitting == true;
}

bool CTaskConversation::process(CharacterState& state, irr::f32 tpf) {
	std::cout << "you sit down" << std::endl;
    state.body->sitting = true;
    return true;
}

bool CTaskConversation::isCompleted(const CharacterState& state) const {
    return state.body->sitting;
}

