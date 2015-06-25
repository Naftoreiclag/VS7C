/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTEROBJECTIVE_H
#define CHARACTEROBJECTIVE_H

#include "CharacterTaskCondition.h"
#include "CharacterTask.h"

class CharacterObjective {
public:
	CharacterObjective();
	virtual ~CharacterObjective();

	CharacterTaskCondition* conditionToFulfill;
	CharacterTask* taskToPerform;
};

#endif // CHARACTEROBJECTIVE_H
