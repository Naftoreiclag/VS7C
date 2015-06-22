/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

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
