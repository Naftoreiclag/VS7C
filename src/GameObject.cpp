#include "GameObject.h"

GameObject::GameObject(artemis::Entity* const entity, irr::scene::ISceneNode* const sceneNode)
: entity(entity),
sceneNode(sceneNode) {
	//ctor
}

GameObject::~GameObject()
{
	//dtor
}
