#include "ChunkNode.h"

using namespace irr;

ChunkNode::ChunkNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id)
: scene::ISceneNode(parent, mgr, id) {
	Material.Lighting = false;

	/*

	1        2


	0        3

	*/

	Vertices[0] = video::S3DVertex(core::vector3df(  0, 0,   0), core::vector3df(0, 1, 0), video::SColor(255,   0, 255,255), core::vector2df(0, 0));
	Vertices[1] = video::S3DVertex(core::vector3df(  0, 0, 100), core::vector3df(0, 1, 0), video::SColor(255,   0, 255,  0), core::vector2df(0, 1));
	Vertices[2] = video::S3DVertex(core::vector3df(100, 0, 100), core::vector3df(0, 1, 0), video::SColor(255, 255, 255,  0), core::vector2df(1, 1));
	Vertices[3] = video::S3DVertex(core::vector3df(100, 0,   0), core::vector3df(0, 1, 0), video::SColor(255, 255,   0,255), core::vector2df(1, 0));

	Box.reset(Vertices[0].Pos);
	for (s32 i=1; i<4; ++i)
		Box.addInternalPoint(Vertices[i].Pos);
}

void ChunkNode::OnRegisterSceneNode() {
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void ChunkNode::render() {
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	u16 indices[6] = {0, 1, 2, 0, 2, 3};

	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawVertexPrimitiveList(&Vertices[0], 4, &indices[0], 2, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
}

const core::aabbox3d<f32>& ChunkNode::getBoundingBox() const {
	return Box;
}

u32 ChunkNode::getMaterialCount() const {
	return 1;
}

video::SMaterial& ChunkNode::getMaterial(u32 i) {
	return Material;
}

