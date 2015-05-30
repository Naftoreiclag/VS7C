#ifndef SCENENODECOMPONENT_H
#define SCENENODECOMPONENT_H

#include "irrlicht.h"
#include "Artemis/Artemis.h"

class SceneNodeComponent : public artemis::Component {
public:
	SceneNodeComponent(irr::scene::ISceneNode* sceneNode);
	~SceneNodeComponent();
	irr::scene::ISceneNode* const sceneNode;
};

#endif // SCENENODECOMPONENT_H
