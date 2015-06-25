/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERTASKSTAND_H
#define CHARACTERTASKSTAND_H

#include "CharacterTaskAtomic.h"

class CharacterTaskStand : public CharacterTaskAtomic {
public:
	CharacterTaskStand();
	virtual ~CharacterTaskStand();

	virtual CharacterTaskStand* clone() const;

	virtual std::vector<CharacterTaskCondition> getPrerequisites()const ;
	virtual std::vector<CharacterTaskCondition> getEffects() const;
	virtual bool fulfills(const CharacterTaskCondition& condition) const;

	virtual bool process(CharacterState& state, irr::f32 tpf);
	virtual bool isCompleted(const CharacterState& state) const;

	// virtual Difficulty getDifficulty() const;
};

#endif // CHARACTERTASKSTAND_H
