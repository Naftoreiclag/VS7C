/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTaskX.h"

CTaskX::CTaskX() {
//	prerequisites.push_back(new ...);
}

CTaskX::~CTaskX() {
	for(std::vector<CharacterTaskCondition*>::iterator it = prerequisites.begin(); it != prerequisites.end(); ++ it) {
		delete *it;
	}
}

CTaskX* CTaskX::newWhichFulfills(const CharacterTaskCondition* condition) const {
	return new CTaskX();
}

std::vector<CharacterTaskCondition*> CTaskX::getPrerequisites() const {
	return prerequisites;
}

bool CTaskX::fulfills(const CharacterTaskCondition* condition) const {
}

bool CTaskX::process(CharacterState& state, irr::f32 tpf) {
}

bool CTaskX::isCompleted(const CharacterState& state) const {
}
