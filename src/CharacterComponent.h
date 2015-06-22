/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHARACTERCOMPONENT_H
#define CHARACTERCOMPONENT_H

#include "Artemis/Artemis.h"

/*
This component defines all character-related information that
applies to both players and non-players, including:
- Name

Must also have either a PlayerComponent or SoulComponent to have any functionality
*/

class CharacterComponent : public artemis::Component {
public:
	std::string name;
	bool sitting;
	CharacterComponent();
	~CharacterComponent();
};
#endif // CHARACTERCOMPONENT_H
