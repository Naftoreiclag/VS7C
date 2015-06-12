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
