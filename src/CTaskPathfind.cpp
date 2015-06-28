/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTaskPathfind.h"

CTaskPathfind::CTaskPathfind(const CTConditionLocation* locationCondition)
: locationCondition(locationCondition) {
}

CTaskPathfind::~CTaskPathfind() {
	for(std::vector<CharacterTaskCondition*>::iterator it = prerequisites.begin(); it != prerequisites.end(); ++ it) {
		delete *it;
	}
}

CTaskPathfind* CTaskPathfind::newWhichFulfills(const CharacterTaskCondition* condition) const {
	return new CTaskPathfind();
}

std::vector<CharacterTaskCondition*> CTaskPathfind::getPrerequisites() const {
	return prerequisites;
}

bool CTaskPathfind::fulfills(const CharacterTaskCondition* condition) const {
	const CTConditionLocation* location = static_cast<const CTConditionLocation*>(condition);
	return location != 0;
}

bool CTaskPathfind::process(CharacterState& state, irr::f32 tpf) {
	CharacterPhysicsComponent* charPhys = state.charPhys;
	PhysicsComponent* phys = state.phys;

	btVector3 disp = locationCondition->getClosestPoint(phys->location) - phys->location;
	disp.normalize();
	disp *= 5;

	charPhys->targetVelocityRelativeToGround = disp;

	return true;
}

bool CTaskPathfind::isCompleted(const CharacterState& state) const {
	return locationCondition->isFulfilled(state);
}
