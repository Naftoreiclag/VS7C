/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "CharacterTask.h"

CharacterTask::CharacterTask(CharacterTaskType type)
: type(type) {
}
CharacterTask::CharacterTask()
: type(UNDEF) {
}

CharacterTask::~CharacterTask()
{
	//dtor
}

CharacterTask::Difficulty CharacterTask::getDifficulty() const {
	return 100;
}
