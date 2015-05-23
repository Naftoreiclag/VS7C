#include "Player.h"
#include "PhysicsComponent.h"

Player::Player(artemis::Entity* const entity, irr::scene::ISceneNode* const sceneNode)
: GameObject::GameObject(entity, sceneNode) {
	physics = new PhysicsComponent();
	entity->addComponent(physics);
	entity->refresh();
}

Player::~Player()
{
}
