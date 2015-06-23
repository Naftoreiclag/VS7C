/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef SOULCOMPONENT_H
#define SOULCOMPONENT_H

#include "NREntitySystem.h"

/*
Marks an entity as non-player-controlled characters,
such as that guy that serves you your coffee.

For characters who do not have a player-granted soul,
here is one that exists in the game's world.
*/
class SoulComponent : public nres::ComponentData
{
public:
	bool happiness;
	SoulComponent();
	virtual SoulComponent* clone() const;
	~SoulComponent();
};

#endif // SOULCOMPONENT_H
