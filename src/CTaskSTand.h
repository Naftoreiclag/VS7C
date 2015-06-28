/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERTASKSTAND_H
#define CHARACTERTASKSTAND_H

#include "CharacterTask.h"

class CharacterTaskStand : public CharacterTask {
public:
	CharacterTaskStand();
	virtual ~CharacterTaskStand();

	virtual std::vector<CharacterTaskCondition*> getPrerequisites()const ;
	virtual bool fulfills(const CharacterTaskCondition* condition) const;
	virtual CharacterTaskStand* newWhichFulfills(const CharacterTaskCondition* condition) const;

	virtual bool process(CharacterState& state, irr::f32 tpf);
	virtual bool isCompleted(const CharacterState& state) const;

	virtual std::string getDebug() {
		return "stand";
	}
	// virtual Difficulty getDifficulty() const;
};

#endif // CHARACTERTASKSTAND_H
