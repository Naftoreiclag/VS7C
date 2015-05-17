#ifndef CHUNKNODE_H
#define CHUNKNODE_H

#include <irrlicht.h>

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

class ChunkNode : public scene::ISceneNode
{
	public:
		ChunkNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id);
		virtual void OnRegisterSceneNode();
		virtual void render();
		virtual const core::aabbox3d<f32>& getBoundingBox() const;
		virtual u32 getMaterialCount() const;
		virtual video::SMaterial& getMaterial(u32 i);
	protected:
	private:
		core::aabbox3d<f32> Box;
		video::S3DVertex Vertices[4];
		video::SMaterial Material;
};

#endif // CHUNKNODE_H
