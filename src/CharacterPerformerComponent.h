/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERPERFORMERCOMPONENT_H
#define CHARACTERPERFORMERCOMPONENT_H

#include "NREntitySystem.h"
#include "CharacterTaskCondition.h"
#include "CharacterTask.h"

/*
Includes only data about what the character is doing at any one time
TODO: add support for doing multiple things at once
*/
class CharacterPerformerComponent : public nres::ComponentData {
public:
	CharacterTaskCondition* currentObjective;
	CharacterTask* currentAction;

	CharacterPerformerComponent();
	virtual CharacterPerformerComponent* clone() const;
	virtual ~CharacterPerformerComponent();
protected:
private:
};

#endif // CHARACTERPERFORMERCOMPONENT_H
