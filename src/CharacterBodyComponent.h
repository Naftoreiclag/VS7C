/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERBODYCOMPONENT_H
#define CHARACTERBODYCOMPONENT_H

#include "NREntitySystem.h"
#include "RID.h"

class CharacterBodyComponent : public nres::ComponentData {
public:
	RID bodyPosition;

	bool sitting = false;

	CharacterBodyComponent();
	virtual CharacterBodyComponent* clone() const;
	virtual ~CharacterBodyComponent();
protected:
private:
};

#endif // CHARACTERBODYCOMPONENT_H
