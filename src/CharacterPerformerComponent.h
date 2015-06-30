/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERPERFORMERCOMPONENT_H
#define CHARACTERPERFORMERCOMPONENT_H

#include <stack>
#include <vector>

#include "irrlicht.h"
#include "NREntitySystem.h"

#include "CharacterObjective.h"
#include "CharacterTask.h"

/*
Includes only data about what the character is doing at any one time
TODO: add support for doing multiple things at once
*/
class CharacterPerformerComponent : public nres::ComponentData {
public:
	struct TaskLayer {
		TaskLayer(CharacterTask* const task);

		CharacterTask* const task;
		std::vector<CharacterTaskCondition*> continuousConditions;
	};
public:
	CharacterObjective currentObjective; // Objective
	std::stack<TaskLayer> taskLayers; // Big stack of tasks
	CharacterTaskCondition* waitingFor = 0; // Waiting for this to be fulfilled first
	irr::f32 timeWaiting = 0; // How long we have been waiting for

	CharacterPerformerComponent();
	virtual CharacterPerformerComponent* clone() const;
	virtual ~CharacterPerformerComponent();
protected:
private:
};

#endif // CHARACTERPERFORMERCOMPONENT_H
