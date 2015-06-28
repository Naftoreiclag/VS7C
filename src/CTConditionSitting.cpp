/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTConditionSitting.h"

CTConditionSitting::CTConditionSitting(bool sitting)
: sitting(sitting) {
}

CTConditionSitting::~CTConditionSitting() {
}

bool CTConditionSitting::isFulfilled(CharacterState state) const {
	return state.body->sitting == sitting;
}
