#include "Player.h"
#include "PhysicsComponent.h"
#include "SceneNodeComponent.h"

Player::Player(artemis::Entity* const entity, irr::scene::ISceneNode* const sceneNode)
: GameObject::GameObject(entity, sceneNode) {
	/*
	physics = new PhysicsComponent();
	entity->addComponent(physics);
	entity->addComponent(new SceneNodeComponent(sceneNode));
	entity->refresh();
	*/
}

Player::~Player()
{
}
