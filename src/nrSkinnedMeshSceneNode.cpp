/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "nrSkinnedMeshSceneNode.h"

nrSkinnedMeshSceneNode::nrSkinnedMeshSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id)
: irr::scene::ISceneNode(parent, mgr, id)
{
	// Don't want to calculate bounding box right now
	setAutomaticCulling(irr::scene::EAC_OFF);
}

nrSkinnedMeshSceneNode::~nrSkinnedMeshSceneNode()
{
	//dtor
}

void nrSkinnedMeshSceneNode::OnRegisterSceneNode() {
}
void nrSkinnedMeshSceneNode::render() {
}
const irr::core::aabbox3d<irr::f32>& nrSkinnedMeshSceneNode::getBoundingBox() const {
}
irr::u32 nrSkinnedMeshSceneNode::getMaterialCount() const {
}
irr::video::SMaterial& nrSkinnedMeshSceneNode::getMaterial(irr::u32 i) {
}
