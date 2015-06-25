/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERTASK_H
#define CHARACTERTASK_H

#include <vector>
#include "CharacterTaskCondition.h"
#include "irrlicht.h"


/*

Base class for all tasks

*/

enum CharacterTaskType {
	ATOMIC, // Indivisible, hard-programmed tasks, favored over others for changing states
	PATHFINDING, // Specialty task that involves moving from one place to another
	COMPLEX // Scripted or otherwise complicated tasks that can be interrrupted
};

// Note:: atomic tasks are derived from this class
class CharacterTask {
public:
	const CharacterTaskType type;

	typedef irr::u32 Difficulty;

	virtual CharacterTask* clone() const = 0;

	virtual void process(irr::f32 tpf) = 0;

	virtual bool isCompleted() = 0;
	virtual std::vector<CharacterTaskCondition> getPrerequisites() = 0;
	virtual std::vector<CharacterTaskCondition> getEffects() = 0;
	virtual bool fulfills(CharacterTaskCondition& condition) = 0;
	virtual Difficulty getDifficulty() = 0;

	CharacterTask(CharacterTaskType type);
	virtual ~CharacterTask();
};

#endif // CHARACTERTASK_H
