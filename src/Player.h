#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Player: public GameObject {
public:
	Player(artemis::Entity* const entity, irr::scene::ISceneNode* const sceneNode);
	virtual ~Player();
};

#endif // PLAYER_H
