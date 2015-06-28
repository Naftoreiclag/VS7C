/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterTaskRegistry.h"
#include "easylogging++.h"

namespace CharacterTaskRegistry {

	std::vector<CharacterTask*> knownTasks;

	void addTask(CharacterTask* task) {
		knownTasks.push_back(task);
	}

	std::vector<CharacterTask*> getTasks(const CharacterTaskCondition* fulfillThis) {
		std::vector<CharacterTask*> retVal;
        for(std::vector<CharacterTask*>::iterator it = knownTasks.begin(); it != knownTasks.end(); ++ it) {
			CharacterTask* task = *it;

			if(task->fulfills(fulfillThis)) {
				LOG(INFO) << task->getDebug();
				retVal.push_back(task);
			}
        }

		LOG(INFO) << retVal.size();

        return retVal;
	}
};
