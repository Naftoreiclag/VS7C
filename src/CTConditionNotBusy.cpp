/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTConditionNotBusy.h"

CTConditionNotBusy::CTConditionNotBusy() {
}

CTConditionNotBusy::~CTConditionNotBusy() {
}

bool CTConditionNotBusy::isFulfilled(const CharacterState& state) const {
	return !state.perf->isBusy();
}
