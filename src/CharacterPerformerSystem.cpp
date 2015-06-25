/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterPerformerSystem.h"



CharacterPerformerSystem::CharacterPerformerSystem() {
	accessedComponents[0] = RID("comp character performer");
	accessedComponents[1] = RID("comp character body");
}
CharacterPerformerSystem::~CharacterPerformerSystem() {
}

const nres::ComponentID* CharacterPerformerSystem::getComponentIDs(std::size_t& numComponentIDs) const {
	numComponentIDs = 2;
    return accessedComponents;
}

void CharacterPerformerSystem::process(nres::Entity& entity) {
    CharacterPerformerComponent* perf = (CharacterPerformerComponent*) entity.getComponentData(RID("comp character performer"));
    CharacterBodyComponent* body = (CharacterBodyComponent*) entity.getComponentData(RID("comp character body"));

    CharacterState state(body);

    if(perf->currentAction) {
        // Check for interruptions
        // Perform current action
    } else if(perf->currentObjective) {
    	// Objective is already fulfilled
		if(perf->currentObjective->isFulfilled(state)) {
			delete perf->currentObjective;
			perf->currentObjective = 0;
		} else {

		}


		// Find next action to fulfill this
    }
}


// Analyze a condition, and determine which immediate task should be performed, compares using analyzeTask()
CharacterPerformerSystem::ConditionReport CharacterPerformerSystem::analyzeCondition(CharacterState state, CharacterTaskCondition* condition) {

	// The report about this condition, including which of task could best fulfill it
	ConditionReport retVal;

	// If condition does not exist for some reason
	if(!condition) {
		retVal.fulfilled = true;
		return retVal;
	}

	// If the condition is already fulfilled
	if(condition->isFulfilled(state)) {
		retVal.fulfilled = true;
		return retVal;
	}

	//
    std::vector<CharacterTask*> taskCandidates;

    std::vector<TaskReport> taskReports;
	for(std::vector<CharacterTask*>::iterator it = taskCandidates.begin(); it != taskCandidates.end(); ++ it) {
		CharacterTask* candidate = *it;

		TaskReport report;
		analyzeTask(state, candidate, report, 5);
		report.subject = candidate;

		taskReports.push_back(report);
	}

	// Find easiest task
	TaskReport* idealTask;
	CharacterTask::Difficulty lowestDifficulty;
	for(std::vector<TaskReport>::iterator it = taskReports.begin(); it != taskReports.end(); ++ it) {
		TaskReport& report = *it;

		if(!idealTask || lowestDifficulty > report.difficulty) {
			idealTask = &report;
			lowestDifficulty = report.difficulty;
		}
	}

	retVal.idealNextStep = idealTask->subject;
	return retVal;
}

// Find the total difficulty of this task and all sub-tasks, assuming all sub-tasks determined by analyzeCondition()
void CharacterPerformerSystem::analyzeTask(CharacterState state, CharacterTask* task, TaskReport& report, int layer) {

	// If the remaining layers of depth reaches zero, then skip analysis and flag report as uncertain (since we never completed the analysis)
	if(layer == 0) {
		report.unsure = true;
		return;
	}

	// Add this task's difficulty
	report.difficulty += task->getDifficulty();

	// Get all the requirements for this condition
	std::vector<CharacterTaskCondition> conditions = task->getPrerequisites();

	// Iterate through all requirements
	for(std::vector<CharacterTaskCondition>::iterator it = conditions.begin(); it != conditions.end(); ++ it) {
		CharacterTaskCondition& condition = *it;

		// Determine what we would hypothetically do in this situation to meet this condition
		ConditionReport condRep = analyzeCondition(state, &condition);

		// If the condition is already fulfilled, then obviously there is no task to be done
		if(condRep.fulfilled) {
			continue;
		}

		// Check for errors here

		// Look ahead further
		analyzeTask(state, condRep.idealNextStep, report, layer - 1);
	}
}
