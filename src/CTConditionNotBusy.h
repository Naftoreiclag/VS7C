/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CTCONDITIONNOTBUSY_H
#define CTCONDITIONNOTBUSY_H

#include "CharacterTaskCondition.h"

/*
The character is currently doing nothing
i.e. not performing a task, and has no objective, and not waiting for anything
*/

class CTConditionNotBusy : public CharacterTaskCondition {
public:
	CTConditionNotBusy();
	virtual ~CTConditionNotBusy();

	virtual bool isFulfilled(const CharacterState& state) const;
};

#endif // CTCONDITIONNOTBUSY_H
