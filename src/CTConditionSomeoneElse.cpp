/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CTConditionSomeoneElse.h"

CTConditionSomeoneElse::CTConditionSomeoneElse(CharacterTaskCondition* const wrapped, nres::Entity& entity)
: wrapped(wrapped),
entity(entity) {

}

CTConditionSomeoneElse::~CTConditionSomeoneElse() {
	delete wrapped;
}

bool CTConditionSomeoneElse::isFulfilled(const CharacterState& state) const {
	CharacterState wrappedState(entity);

	return wrapped->isFulfilled(wrappedState);
}

bool CTConditionSomeoneElse::isValid(const CharacterState& state) const {
	CharacterState wrappedState(entity);

	if(!wrappedState.valid) {
		return false;
	}

	return wrapped->isValid(wrappedState);
}
