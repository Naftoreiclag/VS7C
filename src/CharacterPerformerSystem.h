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
	struct ErrorReport {
		ErrorReport() {
		}

        std::string message; // Error message
	};

	// Describes a conditional condition
	struct ConditionReport {
		ConditionReport()
		: fulfilled(false),
		idealNextStep(0) {
		}

		bool fulfilled; // Condition is already met
		CharacterTask* idealNextStep; // What the next step should be in making the above boolean true (i.e. fulfilling it)
		ErrorReport error; // If there is no next step or this condition is otherwise impossible to fulfill, report an error
	};

	// Describes a conditional task
	struct TaskReport {
		TaskReport()
		: subject(0),
		unsure(false),
		difficulty(0) {
		}

		CharacterTask* subject; // What task is being described here
		bool unsure; // Is any of this data uncertain, (i.e. the task tree could not be fully simulated/predicted)
        CharacterTask::Difficulty difficulty; // Total difficulty

        std::vector<ErrorReport> conditionErrors; // Errors resulting from
	};

	// Analyze a condition, and determine which immediate task should be performed, compares using analyzeTask()
	ConditionReport analyzeCondition(CharacterState state, CharacterTaskCondition* condition);

	// Find the total difficulty of this task and all sub-tasks, assuming all sub-tasks determined by analyzeCondition()
	void analyzeTask(CharacterState state, CharacterTask* task, TaskReport& report, int layer);

	TaskReport* findIdealTask(CharacterState state, std::vector<TaskReport>& reports, bool onlySure, bool onlyPossible);

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
