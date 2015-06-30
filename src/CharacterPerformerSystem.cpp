/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterPerformerSystem.h"

#include <vector>
#include <algorithm>

#include "CharacterTaskRegistry.h"

#include "easylogging++.h"

CharacterPerformerSystem::CharacterPerformerSystem() {
	accessedComponents[0] = RID("comp character performer");
	accessedComponents[1] = RID("comp character body");
	accessedComponents[2] = RID("comp character physics");
	accessedComponents[3] = RID("comp physics");
}
CharacterPerformerSystem::~CharacterPerformerSystem() {
}

const nres::ComponentID* CharacterPerformerSystem::getComponentIDs(std::size_t& numComponentIDs) const {
	numComponentIDs = 4;
    return accessedComponents;
}

void CharacterPerformerSystem::setTpf(irr::f32 value) {
	tpf = value;
}

void CharacterPerformerSystem::process(nres::Entity& entity) {
    CharacterPerformerComponent* perf = (CharacterPerformerComponent*) entity.getComponentData(RID("comp character performer"));

	CharacterState state(entity);
	// Performer is waiting for a condition to be fulfilled
	if(perf->waitingFor) {

		// If waiting is successful
		if(perf->waitingFor->isFulfilled(state)) {
			perf->waitingFor = 0;
		}

		// Condition has not been fulfilled yet
		else {
			// If we have been waiting for too long
			perf->timeWaiting += tpf;
			if(perf->timeWaiting >= perf->waitingFor->waitTimeout) {
				// The whole task tree has failed
				LOG(INFO) << "Condition timed out.";
				perf->waitingFor = 0;
				perf->timeWaiting = 0;
				while(!perf->taskLayers.empty()) {
					delete perf->taskLayers.top().task;
					perf->taskLayers.pop();
				}
				LOG(INFO) << "Deleted task tree.";
			}

			else {
				// [Wait some more...]
				LOG_EVERY_N(1000, INFO) << "Waiting for condition to be fulfilled...";
			}
		}
	}

	// Performer has something to do
	else if(perf->taskLayers.size() > 0) {
		// If this task has something else that needs to be done first, do that
		// TODO: keep track of redirection count
		TaskMetadata nextTask = getNextTask(state, perf->taskLayers.top().task);

		// Something else needs to be done first
		if(nextTask.subject) {

			// If the next task fulfills a continuous condition
			if(nextTask.fulfills->continuous) {
				// Get all the continuous conditions we have fulfilled already
				std::vector<CharacterTaskCondition*> contConds = perf->taskLayers.top().continuousConditions;

				// If we have already tried to fulfill this condition
				if(std::find(contConds.begin(), contConds.end(), nextTask.fulfills) != contConds.end()) {
					// (Then clearly it has been violated, so therefore the task is interrupted)

					LOG(INFO) << "Task interrupted.";
					perf->waitingFor = nextTask.fulfills;
				}

				// This is the first time this continuous condition was fulfilled
				else {
					contConds.push_back(nextTask.fulfills);

					// Do it then
					LOG(INFO) << "Pushing new subtask onto stack";
					perf->taskLayers.push(nextTask.subject->newWhichFulfills(nextTask.fulfills));
				}
			}

			// Next task fulfills something else
			else {
				LOG(INFO) << "Pushing new subtask onto stack";
				perf->taskLayers.push(nextTask.subject->newWhichFulfills(nextTask.fulfills));
			}
		}

		// Nothing else needs to be done first, i.e. do it
		else if(nextTask.except.noSubtaskNeeded) {
			LOG_EVERY_N(1000, INFO) << "No subtask needed. Performing...";
			bool success = perf->taskLayers.top().task->process(state, tpf);

			// Task stepped successfully
			if(success) {

				// Task is complete
				if(perf->taskLayers.top().task->isCompleted(state)) {
					LOG(INFO) << "Task completed.";
					delete perf->taskLayers.top().task;
					perf->taskLayers.pop();
					LOG(INFO) << "Deleted task.";
				}
			}

			// There was an interruption
			else {
				LOG(INFO) << "Task interrupted.";
				perf->waitingFor = nextTask.fulfills;
			}
		}

		// Some problem happened when trying to find the next step in the program
		else {
			LOG(INFO) << "Exception while locating next task.";
			if(nextTask.except.noFulfillmentExists) {
				LOG(INFO) << "No task exists that can fulfill prerequisites.";
			}
			perf->waitingFor = nextTask.fulfills;
		}
	}

	// Performer's objective is to fulfill condition
	else if(perf->currentObjective.conditionToFulfill) {

		// If objective has been fuliflled
        if(perf->currentObjective.conditionToFulfill->isFulfilled(state)) {
			LOG(INFO) << "Objective condtion fulfilled.";
			delete perf->currentObjective.conditionToFulfill;
			perf->currentObjective.conditionToFulfill = 0;
			LOG(INFO) << "Deleted objective condition.";
        }

		// Objective not yet fulfilled
        else {
			LOG(INFO) << "Performer has condition to fulfill.";
        	TaskMetadata nextTask = getNextTask(state, perf->currentObjective.conditionToFulfill);
        	if(nextTask.subject) {
				LOG(INFO) << "Next task located for objective. Setting as next action...";
				perf->taskLayers.push(TaskLayer(nextTask.subject->newWhichFulfills(nextTask.fulfills)));
        	}
        	else {
				LOG(INFO) << "Exception while locating next task.";
				if(nextTask.except.noFulfillmentExists) {
					LOG(INFO) << "No task exists that can fulfill objective condition.";
				}
				delete perf->currentObjective.conditionToFulfill;
				perf->currentObjective.conditionToFulfill = 0;
				LOG(INFO) << "Deleted objective condition.";
        	}
        }
	}

	// Objective is a task
	else if(perf->currentObjective.taskToPerform) {
		LOG(INFO) << "Performer objective task set as current task";
		perf->taskLayers.push(TaskLayer(perf->currentObjective.taskToPerform));
		perf->currentObjective.taskToPerform = 0;
	}
}

CharacterPerformerSystem::TaskMetadata CharacterPerformerSystem::getNextTask(CharacterState state, CharacterTask* taskToPerform) {
	std::vector<CharacterTaskCondition*> prerequisites = taskToPerform->getPrerequisites();

	for(std::vector<CharacterTaskCondition*>::iterator it = prerequisites.begin(); it != prerequisites.end(); ++ it) {
		CharacterTaskCondition* cond = *it;

		if(cond->isFulfilled(state)) {
			continue;
		}

		return getNextTask(state, cond);
	}

	TaskMetadata retVal;
	retVal.subject = 0;
	retVal.fulfills = 0;
	retVal.except.noSubtaskNeeded = true;

	return retVal;
}

CharacterPerformerSystem::TaskMetadata CharacterPerformerSystem::getNextTask(CharacterState state, CharacterTaskCondition* conditionToFulfill) {
	std::vector<CharacterTask*> candidates = CharacterTaskRegistry::getTasks(conditionToFulfill);

	if(candidates.empty()) {
		TaskMetadata retVal;
		retVal.subject = 0;
		retVal.fulfills = conditionToFulfill;
		retVal.except.noFulfillmentExists = true;

		return retVal;
	}

	for(std::vector<CharacterTask*>::iterator it = candidates.begin(); it != candidates.end(); ++ it) {
		CharacterTask* task = *it;

		TaskMetadata retVal;
		retVal.subject = task;
		retVal.fulfills = conditionToFulfill;
		return retVal;
	}

	TaskMetadata retVal;
	retVal.subject = 0;
	retVal.fulfills = conditionToFulfill;
	retVal.except.noFulfillmentExists = true;
	return retVal;
}


