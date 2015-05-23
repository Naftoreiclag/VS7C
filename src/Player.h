#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "PhysicsComponent.h"

class Player: public GameObject {
public:
	Player(artemis::Entity* const entity, irr::scene::ISceneNode* const sceneNode);
	virtual ~Player();

	PhysicsComponent physics;
};

#endif // PLAYER_H
