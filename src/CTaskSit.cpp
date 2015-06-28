/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTaskSit.h"

#include "CTConditionSitting.h"
#include "easylogging++.h"

CharacterTaskSit::CharacterTaskSit()
{
	//ctor
}

CharacterTaskSit::~CharacterTaskSit()
{
	//dtor
}

std::vector<CharacterTaskCondition*> CharacterTaskSit::getPrerequisites() const {
	std::vector<CharacterTaskCondition*> retVal;
	CharacterTaskCondition* condition = new CTConditionSitting(false);
	retVal.push_back(condition);
	return retVal;
}

CharacterTaskSit* CharacterTaskSit::newWhichFulfills(const CharacterTaskCondition* condition) const {
	return new CharacterTaskSit();
}

bool CharacterTaskSit::fulfills(const CharacterTaskCondition* condition) const {
	const CTConditionSitting* sitting = dynamic_cast<const CTConditionSitting*>(condition);
	if(sitting) {
		return sitting->sitting == true;
	}

	return false;
}

bool CharacterTaskSit::process(CharacterState& state, irr::f32 tpf) {
	LOG(INFO) << "you sit down";
    state.body->sitting = true;
    return true;
}

bool CharacterTaskSit::isCompleted(const CharacterState& state) const {
    return state.body->sitting;
}
