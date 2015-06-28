/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTaskPathfind.h"

CTaskPathfind::CTaskPathfind() {
//	prerequisites.push_back(new ...);
}

CTaskPathfind::~CTaskPathfind() {
	for(std::vector<CharacterTaskCondition*>::iterator it = prerequisites.begin(); it != prerequisites.end(); ++ it) {
		delete *it;
	}
	for(std::vector<CharacterTaskCondition*>::iterator it = effects.begin(); it != effects.end(); ++ it) {
		delete *it;
	}
}

CTaskPathfind* CTaskPathfind::clone() const {
	return new CTaskPathfind();
}


std::vector<CharacterTaskCondition*> CTaskPathfind::getPrerequisites() const {
	std::vector<CharacterTaskCondition*> retVal;
	CharacterTaskCondition* condition = new CTConditionSitting(true);
	retVal.push_back(condition);
	return retVal;
}

std::vector<CharacterTaskCondition*> CTaskPathfind::getEffects() const {
	std::vector<CharacterTaskCondition*> retVal;
	CharacterTaskCondition* condition = new CTConditionSitting(false);
	retVal.push_back(condition);
	return retVal;
}

bool CTaskPathfind::fulfills(const CharacterTaskCondition* condition) const {
}

bool CTaskPathfind::process(CharacterState& state, irr::f32 tpf) {
}

bool CTaskPathfind::isCompleted(const CharacterState& state) const {
}
