/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERTASKSIT_H
#define CHARACTERTASKSIT_H

#include "CharacterTaskAtomic.h"

class CharacterTaskSit : public CharacterTaskAtomic {
public:
	CharacterTaskSit();
	virtual void process(irr::f32 tpf);
	virtual ~CharacterTaskSit();
};

#endif // CHARACTERTASKSIT_H
