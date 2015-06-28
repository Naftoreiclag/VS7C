/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTaskConversation.h"

#include <iostream>

CTaskConversation::CTaskConversation(nres::Entity& talkTo)
: talkTo(talkTo) {
}

CTaskConversation::~CTaskConversation()
{
	//dtor
}

CTaskConversation* CTaskConversation::newWhichFulfills(const CharacterTaskCondition* condition) const {

	return new CTaskConversation(talkTo);
}


std::vector<CharacterTaskCondition*> CTaskConversation::getPrerequisites() const {
	std::vector<CharacterTaskCondition*> retVal;
	return retVal;
}

bool CTaskConversation::fulfills(const CharacterTaskCondition* condition) const {
	return false;
}

bool CTaskConversation::process(CharacterState& state, irr::f32 tpf) {
	std::cout << "you sit down" << std::endl;
    state.body->sitting = true;
    return true;
}

bool CTaskConversation::isCompleted(const CharacterState& state) const {
    return state.body->sitting;
}

