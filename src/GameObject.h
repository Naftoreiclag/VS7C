#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Artemis/Artemis.h"
#include "irrlicht.h"

// Serializable stuff
class GameObject {
public:
	GameObject(artemis::Entity* const entity, irr::scene::ISceneNode* const sceneNode);
	virtual ~GameObject();

	// Constant pointers to some stuff
    artemis::Entity* const entity;
    irr::scene::ISceneNode* const sceneNode;
};

#endif // GAMEOBJECT_H
