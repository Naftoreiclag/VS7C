#include "Player.h"
#include "PhysicsComponent.h"

Player::Player(artemis::Entity* const entity, irr::scene::ISceneNode* const sceneNode)
: GameObject::GameObject(entity, sceneNode)
{
	entity->addComponent(new PhysicsComponent());
	entity->refresh();
}

Player::~Player()
{
	//dtor
}
