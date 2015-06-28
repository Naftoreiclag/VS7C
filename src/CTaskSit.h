/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERTASKSIT_H
#define CHARACTERTASKSIT_H

#include "CharacterTask.h"

class CharacterTaskSit : public CharacterTask {
public:
	CharacterTaskSit();
	virtual ~CharacterTaskSit();

	virtual CharacterTaskSit* clone() const;

	virtual std::vector<CharacterTaskCondition*> getPrerequisites()const ;
	virtual std::vector<CharacterTaskCondition*> getEffects() const;
	virtual bool fulfills(const CharacterTaskCondition* condition) const;

	virtual bool process(CharacterState& state, irr::f32 tpf);
	virtual bool isCompleted(const CharacterState& state) const;

	// virtual Difficulty getDifficulty() const;
};

#endif // CHARACTERTASKSIT_H
