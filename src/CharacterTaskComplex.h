/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERTASKCOMPLEX_H
#define CHARACTERTASKCOMPLEX_H

#include "CharacterTask.h"

class CharacterTaskComplex : public CharacterTask {
public:
	bool automatic; // Whether or not the task proceeds without player input

	CharacterTaskComplex();
	virtual ~CharacterTaskComplex();
};

#endif // CHARACTERTASKCOMPLEX_H
