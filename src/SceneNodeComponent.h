/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef SCENENODECOMPONENT_H
#define SCENENODECOMPONENT_H

#include "irrlicht.h"

#include "BaseComponent.h"
#include "NREntitySystem.h"

/*
Basic scene node data, nothing else really
*/
class SceneNodeComponent : public BaseComponent {
public:
	SceneNodeComponent(irr::scene::ISceneNode* sceneNode);
	virtual SceneNodeComponent* clone() const;
	~SceneNodeComponent();
	irr::scene::ISceneNode* const sceneNode;
};

#endif // SCENENODECOMPONENT_H
