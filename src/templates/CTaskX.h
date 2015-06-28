/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CTASKX_H
#define CTASKX_H

#include "CharacterTask.h"

class CTaskX : public CharacterTask {
private:
	std::vector<CharacterTaskCondition*> prerequisites;
	std::vector<CharacterTaskCondition*> effects;
public:
	CTaskX();
	virtual ~CTaskX();

	virtual CTaskX* clone() const;
	

	virtual std::vector<CharacterTaskCondition*> getPrerequisites()const;
	virtual std::vector<CharacterTaskCondition*> getEffects() const;
	virtual bool fulfills(const CharacterTaskCondition* condition) const;

	virtual bool process(CharacterState& state, irr::f32 tpf);
	virtual bool isCompleted(const CharacterState& state) const;

	// virtual Difficulty getDifficulty() const;
};

#endif // CTASKX_H
