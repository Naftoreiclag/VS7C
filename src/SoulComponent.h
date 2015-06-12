#ifndef SOULCOMPONENT_H
#define SOULCOMPONENT_H

#include "Artemis/Artemis.h"

/*
Marks an entity as non-player-controlled characters,
such as that guy that serves you your coffee.

For characters who do not have a player-granted soul,
here is one that exists in the game's world.
*/
class SoulComponent : public artemis::Component
{
public:
	bool happiness;
	SoulComponent();
	~SoulComponent();
};

#endif // SOULCOMPONENT_H
