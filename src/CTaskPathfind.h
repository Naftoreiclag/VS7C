/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CTASKX_H
#define CTASKX_H

#include "CharacterTask.h"
#include "CTConditionLocation.h"

class CTaskPathfind : public CharacterTask {
private:
	std::vector<CharacterTaskCondition*> prerequisites;
public:
	CTaskPathfind(const CTConditionLocation* locationCondition = 0);
	virtual ~CTaskPathfind();


	const CTConditionLocation* locationCondition;


	virtual std::vector<CharacterTaskCondition*> getPrerequisites()const;
	virtual bool fulfills(const CharacterTaskCondition* condition) const;
	virtual CTaskPathfind* newWhichFulfills(const CharacterTaskCondition* condition) const;

	virtual bool process(CharacterState& state, irr::f32 tpf);
	virtual bool isCompleted(const CharacterState& state) const;

	// virtual Difficulty getDifficulty() const;
};

#endif // CTASKX_H
