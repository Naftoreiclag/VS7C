/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERCOMPONENT_H
#define CHARACTERCOMPONENT_H

#include "NREntitySystem.h"

/*
This component defines all character-related information that
applies to both players and non-players, including:
- Name

Must also have either a PlayerComponent or SoulComponent to have any functionality
*/

class CharacterComponent : public nres::ComponentData {
public:
	std::string name;
	bool sitting;
	virtual CharacterComponent* clone() const;
	CharacterComponent();
	~CharacterComponent();
};
#endif // CHARACTERCOMPONENT_H
