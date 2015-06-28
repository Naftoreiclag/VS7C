/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTConditionLocation.h"

CTConditionLocation::CTConditionLocation(const btScalar dist, nres::Entity& other)
: dist(dist),
entity(&other),
type(NEXT_TO_ENTITY) {
    otherPhys = (PhysicsComponent*) entity->getComponentData(RID("comp physics"));
}
CTConditionLocation::CTConditionLocation(const btScalar dist, const btVector3 loc)
: dist(dist),
loc(loc),
type(NEXT_TO_POINT) {
}

CTConditionLocation::~CTConditionLocation() {

}

bool CTConditionLocation::isFulfilled(CharacterState state) const {
	if(type == NEXT_TO_ENTITY) {
		return state.phys->location.distance2(otherPhys->location) <= dist * dist;
	}
	else if(type == NEXT_TO_POINT) {
		return state.phys->location.distance2(loc) <= dist * dist;
	}

	return true;
}

bool CTConditionLocation::isPossible(CharacterState state) {
	if(type == NEXT_TO_ENTITY) {
		return otherPhys != 0;
	}
	else {
		return true;
	}
}
