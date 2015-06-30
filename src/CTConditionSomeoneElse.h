/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CTCONDITIONSOMEONEELSE_H
#define CTCONDITIONSOMEONEELSE_H

#include "CharacterTaskCondition.h"

#include "nresEntity.h"

/*
Wraps a condition to apply to someone else.
There should not be a task to cause this. (Maybe forcing someone to do something will be a later feature???)
Will delete wrapped condition automatically.
*/

class CTConditionSomeoneElse : public CharacterTaskCondition {
public:
	CTConditionSomeoneElse(nres::Entity& entity, CharacterTaskCondition* const wrapped);
	virtual ~CTConditionSomeoneElse();

	CharacterTaskCondition* const wrapped;
	nres::Entity& entity;

	virtual bool isFulfilled(const CharacterState& state) const;
	virtual bool isValid(const CharacterState& state) const;
};

#endif // CTCONDITIONSOMEONEELSE_H
