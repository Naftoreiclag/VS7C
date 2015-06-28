/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterPerformerSystem.h"

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
    CharacterBodyComponent* body = (CharacterBodyComponent*) entity.getComponentData(RID("comp character body"));
    CharacterPhysicsComponent* charPhys = (CharacterPhysicsComponent*) entity.getComponentData(RID("comp character physics"));
    PhysicsComponent* phys = (PhysicsComponent*) entity.getComponentData(RID("comp physics"));

	CharacterState state(body, charPhys, phys);
	if(perf->currentAction) {
		// If this task has something else that needs to be done first, do that
		// TODO: keep track of redirection count
		TaskMetadata nextTask = getNextTask(state, perf->currentAction);
		// This task has something else that needs to be done first
		if(nextTask.subject) {
			LOG(INFO) << "Switching to subtask.";
			// Replace the current action with the next one
			delete perf->currentAction;
			perf->currentAction = 0;
			perf->currentAction = nextTask.subject->newWhichFulfills(nextTask.fulfills);
		}

		// This task has nothing else that needs to be done first
		else if(nextTask.except.noSubtaskNeeded) {
			LOG_EVERY_N(100, INFO) << "No subtask needed. Performing...";
			bool success = perf->currentAction->process(state, tpf);
			if(success) {
				if(perf->currentAction->isCompleted(state)) {
					LOG(INFO) << "Task completed.";
					if(perf->currentAction == perf->currentObjective.taskToPerform) {
						perf->currentObjective.taskToPerform = 0;
					}
					delete perf->currentAction;
					perf->currentAction = 0;
					LOG(INFO) << "Deleted task.";
				}
			}
			else {
				LOG(INFO) << "Task interrupted.";
				if(perf->currentAction == perf->currentObjective.taskToPerform) {
					perf->currentObjective.taskToPerform = 0;
				}
				delete perf->currentAction;
				perf->currentAction = 0;
				LOG(INFO) << "Deleted task.";
			}
		}

		// Some other problem happened
		else {
			LOG(INFO) << "Exception while locating next task.";
			if(nextTask.except.noFulfillmentExists) {
				LOG(INFO) << "No task exists that can fulfill objective condition.";
			}
			delete perf->currentObjective.taskToPerform;
			perf->currentObjective.taskToPerform = 0;
			LOG(INFO) << "Deleted objective task.";
		}
	}

	// Objective is a condition
	else if(perf->currentObjective.conditionToFulfill) {
        if(perf->currentObjective.conditionToFulfill->isFulfilled(state)) {
			LOG(INFO) << "Objective condtion fulfilled.";
			delete perf->currentObjective.conditionToFulfill;
			perf->currentObjective.conditionToFulfill = 0;
			LOG(INFO) << "Deleted objective condition.";
        }
        else {
			LOG(INFO) << "Performer has condition to fulfill.";
        	TaskMetadata nextTask = getNextTask(state, perf->currentObjective.conditionToFulfill);
        	if(nextTask.subject) {
				LOG(INFO) << "Next task located for objective. Setting as next action...";
				perf->currentAction = nextTask.subject->newWhichFulfills(nextTask.fulfills);
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
		TaskMetadata nextTask = getNextTask(state, perf->currentObjective.taskToPerform);
		if(nextTask.subject) {
			LOG(INFO) << "Next task located for objective. Setting as next action...";
			perf->currentAction = nextTask.subject->newWhichFulfills(nextTask.fulfills);
		}
		else if(nextTask.except.noSubtaskNeeded) {
			LOG(INFO) << "All conditions met for objective task. Setting as next action...";
			perf->currentAction = perf->currentObjective.taskToPerform;
		}
		else {
			LOG(INFO) << "Exception while locating next task.";
			if(nextTask.except.noFulfillmentExists) {
				LOG(INFO) << "No task exists that can fulfill objective condition.";
			}
			perf->currentObjective.taskToPerform = 0;
			delete perf->currentObjective.taskToPerform;
			LOG(INFO) << "Deleted objective task.";
		}
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


