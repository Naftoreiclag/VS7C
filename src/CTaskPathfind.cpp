/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTaskPathfind.h"
#include "CTConditionSitting.h"
#include "easylogging++.h"

CTaskPathfind::CTaskPathfind(const CTConditionLocation* locationCondition)
: locationCondition(locationCondition) {
	CTConditionSitting* sitt = new CTConditionSitting(false);
	prerequisites.push_back(sitt);
}

CTaskPathfind::~CTaskPathfind() {
	for(std::vector<CharacterTaskCondition*>::iterator it = prerequisites.begin(); it != prerequisites.end(); ++ it) {
		delete *it;
	}
}

CTaskPathfind* CTaskPathfind::newWhichFulfills(const CharacterTaskCondition* condition) const {
	return new CTaskPathfind(dynamic_cast<const CTConditionLocation*>(condition));
}

std::vector<CharacterTaskCondition*> CTaskPathfind::getPrerequisites() const {
	return prerequisites;
}

bool CTaskPathfind::fulfills(const CharacterTaskCondition* condition) const {
	const CTConditionLocation* location = dynamic_cast<const CTConditionLocation*>(condition);
	return location != 0;
}

bool CTaskPathfind::process(CharacterState& state, irr::f32 tpf) {
	CharacterPhysicsComponent* charPhys = state.charPhys;
	PhysicsComponent* phys = state.phys;

	btVector3 disp = locationCondition->getClosestPoint(phys->location) - phys->location;
	disp.normalize();
	disp *= 5;

	charPhys->targetVelocityRelativeToGround = disp;
	charPhys->isWalking = true;

	return true;
}

bool CTaskPathfind::isCompleted(const CharacterState& state) const {
	return locationCondition->isFulfilled(state);
}
