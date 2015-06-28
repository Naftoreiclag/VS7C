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

// Note:: atomic tasks are derived from this class
class CharacterTask {
public:
	CharacterTask();
	virtual ~CharacterTask();


	virtual std::vector<CharacterTaskCondition*> getPrerequisites() const = 0; // What conditions are required before this task can begin
	virtual bool fulfills(const CharacterTaskCondition* condition) const = 0; // Would doing this task fufill this condition
	virtual CharacterTask* newWhichFulfills(const CharacterTaskCondition* condition) const = 0; // Creates a new instance of this class constructed specifically to fulfill given condition

	virtual bool process(CharacterState& state, irr::f32 tpf) = 0; // Returns true iff there was no interruption
	virtual bool isCompleted(const CharacterState& state) const = 0;

	virtual std::string getDebug() {
		return "errr";
	}

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
