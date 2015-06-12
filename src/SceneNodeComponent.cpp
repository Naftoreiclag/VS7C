#include "SceneNodeComponent.h"

SceneNodeComponent::SceneNodeComponent(irr::scene::ISceneNode* sceneNode)
: artemis::Component(),
sceneNode(sceneNode) {
	sceneNode->grab();
}

SceneNodeComponent::~SceneNodeComponent() {
	sceneNode->remove();
	sceneNode->drop();
}
