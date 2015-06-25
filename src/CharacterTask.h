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
	CharacterTask(CharacterTaskType type);
	virtual ~CharacterTask();

	const CharacterTaskType type;
	virtual CharacterTask* clone() const = 0;

	virtual std::vector<CharacterTaskCondition> getPrerequisites() const = 0;
	virtual std::vector<CharacterTaskCondition> getEffects() const = 0;
	virtual bool fulfills(const CharacterTaskCondition& condition) const = 0;

	typedef CharacterTaskCondition::CharacterState CharacterState;
	virtual void process(CharacterState& state, irr::f32 tpf) = 0;
	virtual bool isCompleted(const CharacterState& state) const = 0;

	/*
	Example difficulty levels:
	0. blinking, turning head, breathing, chewing
	1. walking
	2. standing, sitting

	*/

	typedef irr::u32 Difficulty;
	virtual Difficulty getDifficulty() const;

};

#endif // CHARACTERTASK_H
