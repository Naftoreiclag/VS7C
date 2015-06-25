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
public:
	typedef CharacterTaskCondition::CharacterState CharacterState;
private:
	struct ConditionReport {
		ConditionReport()
		: fulfilled(false),
		idealNextStep(0) {
		}
		bool fulfilled;

		CharacterTask* idealNextStep;
		// Error messages: unfulfillable, unknown process... etc
	};
	struct TaskReport {
		TaskReport()
		: subject(0),
		unsure(false),
		difficulty(0) {
		}
		CharacterTask* subject;
		bool unsure;
        CharacterTask::Difficulty difficulty;
        // vector of noteworthy requirements
	};

	// Analyze a condition, and determine which immediate task should be performed, compares using analyzeTask()
	ConditionReport analyzeCondition(CharacterState state, CharacterTaskCondition* condition);

	// Find the total difficulty of this task and all sub-tasks, assuming all sub-tasks determined by analyzeCondition()
	void analyzeTask(CharacterState state, CharacterTask* task, TaskReport& report, int layer);

	//
	irr::f32 tpf;
private:
	nres::ComponentID accessedComponents[2];
public:
	CharacterPerformerSystem();
	~CharacterPerformerSystem();
	void setTpf(irr::f32 tpf);
	virtual const nres::ComponentID* getComponentIDs(std::size_t& numComponentIDs) const;
	virtual void process(nres::Entity& entity);
};


#endif // CHARACTERPERFORMERSYSTEM_H
