/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef NRSKINNEDMESHSCENENODE_H
#define NRSKINNEDMESHSCENENODE_H

#include "irrlicht.h"

class nrSkinnedMeshSceneNode : public irr::scene::ISceneNode {
public:
	irr::core::aabbox3d<irr::f32> boundingBox;
	irr::video::S3DVertex Vertices[4];
	irr::video::SMaterial Material;

	nrSkinnedMeshSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id);
	virtual ~nrSkinnedMeshSceneNode();

	virtual void OnRegisterSceneNode();
	virtual void render();
	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
	virtual irr::u32 getMaterialCount() const;
	virtual irr::video::SMaterial& getMaterial(irr::u32 i);
protected:
private:
};

#endif // NRSKINNEDMESHSCENENODE_H
