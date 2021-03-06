/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTaskConversation.h"
#include "CTConditionLocation.h"
#include "CTConditionSomeoneElse.h"
#include "CTConditionNotBusy.h"

#include <iostream>

CTaskConversation::CTaskConversation(nres::Entity& talkTo)
: talkTo(talkTo) {
	// Target must not be busy doing something else
	CTConditionSomeoneElse* otherIsNotBusy = new CTConditionSomeoneElse(talkTo, new CTConditionNotBusy());
	otherIsNotBusy->continuous = true;
	prerequisites.push_back(otherIsNotBusy);

	// Must be within 5 feet of target
	prerequisites.push_back(new CTConditionLocation(5, talkTo));

	// Mispelt temp var
	converationDone = false;
}

CTaskConversation::~CTaskConversation() {
	for(std::vector<CharacterTaskCondition*>::iterator it = prerequisites.begin(); it != prerequisites.end(); ++ it) {
		delete *it;
	}
}

// Should not be called
CTaskConversation* CTaskConversation::newWhichFulfills(const CharacterTaskCondition* condition) const {
	return new CTaskConversation(talkTo);
}

std::vector<CharacterTaskCondition*> CTaskConversation::getPrerequisites() const {
	return prerequisites;
}

bool CTaskConversation::fulfills(const CharacterTaskCondition* condition) const {
	return false; // No associated condition
}

bool CTaskConversation::process(CharacterState& state, irr::f32 tpf) {
	converationDone = true;
	return true;
}

bool CTaskConversation::isCompleted(const CharacterState& state) const {
	return converationDone;
}

