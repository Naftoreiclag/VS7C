#include "SceneNodeComponent.h"

SceneNodeComponent::SceneNodeComponent(irr::scene::ISceneNode* sceneNode)
: sceneNode(sceneNode)
{
	sceneNode->grab();
}

SceneNodeComponent::~SceneNodeComponent() {
	sceneNode->drop();
}
