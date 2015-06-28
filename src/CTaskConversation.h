/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CTASKCONVERSATION_H
#define CTASKCONVERSATION_H

#include "nresEntity.h"
#include "CharacterTask.h"

class CTaskConversation : public CharacterTask {
public:
	CTaskConversation(nres::Entity& talkTo);
	virtual ~CTaskConversation();


	virtual std::vector<CharacterTaskCondition*> getPrerequisites()const;
	virtual bool fulfills(const CharacterTaskCondition* condition) const;
	virtual CTaskConversation* newWhichFulfills(const CharacterTaskCondition* condition) const;

	virtual bool process(CharacterState& state, irr::f32 tpf);
	virtual bool isCompleted(const CharacterState& state) const;
private:
	nres::Entity& talkTo;
};

#endif // CTASKCONVERSATION_H
