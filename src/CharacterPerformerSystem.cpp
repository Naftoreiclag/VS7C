/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterPerformerSystem.h"

#include "CharacterTaskRegistry.h"
#include <iostream>

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

void CharacterPerformerSystem::setTpf(irr::f32 value) {
	tpf = value;
}

void CharacterPerformerSystem::process(nres::Entity& entity) {
    CharacterPerformerComponent* perf = (CharacterPerformerComponent*) entity.getComponentData(RID("comp character performer"));
    CharacterBodyComponent* body = (CharacterBodyComponent*) entity.getComponentData(RID("comp character body"));


	CharacterState state(body);
    if(perf->currentAction) {
        // Perform current action
		bool success = perf->currentAction->process(state, tpf);

		// Check for completion, delete if completed
		if(perf->currentAction->isCompleted(state)) {
			delete perf->currentAction;
			perf->currentAction = 0;
		}
    } else if(perf->currentObjective) {
    	ConditionReport report = analyzeCondition(state, perf->currentObjective);

		// Fulfilled already
    	if(report.fulfilled) {
			perf->currentObjective = 0;
    	}

    	// Setup action
    	else {
			perf->currentAction = report.idealNextStep->clone();
    	}
    }
}


// Analyze a condition, and determine which immediate task should be performed, compares using analyzeTask()
CharacterPerformerSystem::ConditionReport CharacterPerformerSystem::analyzeCondition(CharacterState state, CharacterTaskCondition* condition) {

	// The report about this condition, including which of task could best fulfill it
	ConditionReport retVal;

	std::cout << "checking fulfillment" << std::endl;

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

	// Get all possible tasks that can fufill this condition
    std::vector<CharacterTask*> taskCandidates = CharacterTaskRegistry::getTasks(*condition);

    // If there are no candidates, throw an exception
    if(taskCandidates.empty()) {
		retVal.idealNextStep = 0;
		retVal.error.message = "Does not know how to fulfill %condition%";
		return retVal;
    }

	// Keep track of reports for each candidate so that the optimal task can be chosen
    std::vector<TaskReport> taskReports;
	for(std::vector<CharacterTask*>::iterator it = taskCandidates.begin(); it != taskCandidates.end(); ++ it) {
		CharacterTask* candidate = *it;

		// Generate a report for each potential task, most importantly evaluating the difficulty of each
		TaskReport report;
		analyzeTask(state, candidate, report, 5);
		report.subject = candidate;

		// Keep track of reports
		taskReports.push_back(report);
	}

	// Find the ideal task
	TaskReport* idealTask;
	idealTask = findIdealTask(state, taskReports, true, true); // Including only possible ones and sure ones

	// No certain task exists
	if(!idealTask) {
		// Try find a task that is not certain but might still be possible
		idealTask = findIdealTask(state, taskReports, false, true);

		// No task exists
		if(!idealTask) {
			// Find the best task to do, if it were possible
			idealTask = findIdealTask(state, taskReports, false, false);

			// (idealTask must exist here, since we know the number of candidates > 0 from the previous check)
			retVal.idealNextStep = 0;
			retVal.error.message = "Cannot do %task% because %reasons%"; // use some info from idealTask up there
			return retVal;
		}
	}

	// Everything is okay and the ideal next step has been determined
	retVal.idealNextStep = idealTask->subject;
	return retVal;
}

CharacterPerformerSystem::TaskReport* CharacterPerformerSystem::findIdealTask(CharacterState state, std::vector<TaskReport>& reports, bool onlySure, bool onlyPossible) {
	TaskReport* idealTask = 0;
	for(std::vector<TaskReport>::iterator it = reports.begin(); it != reports.end(); ++ it) {
		TaskReport& report = *it;

		// If there are errors in this task and we only accept possible ones
		if(onlyPossible && !report.conditionErrors.empty()) { continue; }

		// If the certainty of this report is not acceptable
		if(onlySure && !report.unsure) { continue; }

		// If this is the first task we examine, then it is the best one so far
		if(!idealTask) {
			idealTask = &report;
		}

		// If this new task has a lower difficulty
		if(idealTask->difficulty > report.difficulty) {
			idealTask = &report;
		}
	}

	return idealTask;
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

		// If the condition is unfulfillable
		if(!condRep.idealNextStep) {
			report.unsure = true;
			report.conditionErrors.push_back(condRep.error);
			continue;
		}

		// Look ahead further
		analyzeTask(state, condRep.idealNextStep, report, layer - 1);
	}
}
