/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERTASKREGISTRY_H
#define CHARACTERTASKREGISTRY_H

#include "CharacterTask.h"
#include <vector>

namespace CharacterTaskRegistry {

	void addTask(CharacterTask* task);

	// Get a vector of all the tasks that have that effect
	std::vector<CharacterTask*> getTasks(const CharacterTaskCondition* fulfillThis);
};

#endif // CHARACTERTASKREGISTRY_H
