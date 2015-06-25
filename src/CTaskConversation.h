/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CTASKCONVERSATION_H
#define CTASKCONVERSATION_H

#include "CharacterTask.h"

class CTaskConversation : public CharacterTask {
public:
	CTaskConversation();
	virtual ~CTaskConversation();

	virtual CTaskConversation* clone() const;

	virtual std::vector<CharacterTaskCondition> getPrerequisites()const ;
	virtual std::vector<CharacterTaskCondition> getEffects() const;
	virtual bool fulfills(const CharacterTaskCondition& condition) const;

	virtual bool process(CharacterState& state, irr::f32 tpf);
	virtual bool isCompleted(const CharacterState& state) const;
};

#endif // CTASKCONVERSATION_H
