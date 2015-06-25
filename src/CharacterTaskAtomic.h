/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERTASKATOMIC_H
#define CHARACTERTASKATOMIC_H

#include "CharacterTask.h"

class CharacterTaskAtomic : public CharacterTask {
public:
	virtual void process(irr::f32 tpf) = 0;

	CharacterTaskAtomic();
	virtual ~CharacterTaskAtomic();
};

#endif // CHARACTERTASKATOMIC_H
