/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERPERFORMERSYSTEM_H
#define CHARACTERPERFORMERSYSTEM_H

#include "NREntitySystem.h"

#include "CharacterTaskCondition.h"
#include "RID.h"
#include "irrlicht.h"

#include "CharacterPerformerComponent.h"

class CharacterPerformerSystem : public nres::System {
private:
	// If a TaskMetadata has subject == 0, then this details why
	struct TaskException {
		// When a task is asked to give another task to be done beforehand, but no subtask is necessary
		bool noSubtaskNeeded = false;

		// When a condition cannot return a task because none exists
		bool noFulfillmentExists = false;
	};
	struct TaskMetadata {
		CharacterTask* subject = 0;
		TaskException except;
	};

	TaskMetadata getNextTask(CharacterState state, CharacterTask* taskToPerform);
	TaskMetadata getNextTask(CharacterState state, CharacterTaskCondition* conditionToFulfill);

	//
	irr::f32 tpf;
private:
	nres::ComponentID accessedComponents[4];
public:
	CharacterPerformerSystem();
	~CharacterPerformerSystem();
	void setTpf(irr::f32 tpf);
	virtual const nres::ComponentID* getComponentIDs(std::size_t& numComponentIDs) const;
	virtual void process(nres::Entity& entity);
};


#endif // CHARACTERPERFORMERSYSTEM_H
