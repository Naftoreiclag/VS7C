#ifndef SCENENODECOMPONENT_H
#define SCENENODECOMPONENT_H

#include "irrlicht.h"
#include "Artemis/Artemis.h"

class SceneNodeComponent : public artemis::Component {
public:
	SceneNodeComponent(irr::scene::ISceneNode* sceneNode);
	irr::scene::ISceneNode* const sceneNode;
};

/*
class SceneNodeSystem : public artemis::EntityProcessingSystem {
private:
	artemis::ComponentMapper<SceneNodeComponent> sceneNodeMapper;

public:
	PhysicsSystem() {
		addComponentType<SceneNodeComponent>();
		addComponentType<PhysicsComponent>();

	}
	virtual void initialize() {
		sceneNodeMapper.init(*world);
	}
	virtual void processEntity(artemis::Entity& e) {

		SceneNodeComponent* comp = scenenNodeMapper.get(e);
		sceneNodeMapper.get(e)->x += sceneNodeMapper.get(e)->velX * world->getDelta();
		sceneNodeMapper.get(e)->z += sceneNodeMapper.get(e)->velZ * world->getDelta();

	}

};
*/

#endif // SCENENODECOMPONENT_H
