#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "Artemis/Artemis.h"

/*
Marks an entity as player-controlled
*/
class PlayerComponent : public artemis::Component
{
	public:
		PlayerComponent();
		~PlayerComponent();
	protected:
	private:
};

#endif // PLAYERCOMPONENT_H
