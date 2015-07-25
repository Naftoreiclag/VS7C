/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef REIASSIMP_H
#define REIASSIMP_H

#include <iostream>
#include <string>

#include "irrlicht.h"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"
#include "assimp/anim.h"

namespace reia {
	// Extra per-vertex data
	struct VertexMetadata {
		// These numbers are indices in the usedBones[] array
		irr::u8 boneW = 255;
		irr::u8 boneX = 255;
		irr::u8 boneY = 255;
		irr::u8 boneZ = 255;

		irr::f32 weightW;
		irr::f32 weightX;
		irr::f32 weightY;
		irr::f32 weightZ;
	};

	// Describes
	struct BoneMetadata {
		std::string boneName;
		irr::core::matrix4 offsetMatrix;
	};

	// Extra data for each buffer (group of vertices that has the same material or otherwise differenciated from the rest of the mesh)
	struct BufferMetadata {
		// Bones that influences this group of vertices
		BoneMetadata* usedBones = 0;
		irr::u32 numBones;

		// The metadata to apply to each vertex
		VertexMetadata* verts = 0;
		irr::u32 numVerts;
	};
	struct VectorKey {
		irr::core::vector3df value;
		irr::f32 time;
	};
	struct QuaternionKey {
		irr::core::quaternion value;
		irr::f32 time;
	};
	struct ChannelData {
		std::string boneName;

        VectorKey* positions;
        irr::u32 numPositions = 0;

        QuaternionKey* rotations;
        irr::u32 numRotations = 0;

        VectorKey* scalings;
        irr::u32 numScalings = 0;
	};
	struct AnimationData {
		std::string animName;
		irr::f32 duration;
		irr::f32 tps;

		ChannelData* channels = 0;
		irr::u32 numChannels = 0;
	};

	struct Bone {
        std::string name;

        bool isRoot = false;
        irr::u32 parentId;
        irr::u32* childrenIds = 0;
        irr::u32 numChildren = 0;

        irr::core::matrix4 trans;
	};

	// A single mesh with some number of bones and animations
	struct ComplexMeshData {
		irr::scene::SMesh* mesh = 0;

		BufferMetadata* buffers = 0;
		irr::u32 numBuffers = 0;

		AnimationData* anims = 0;
		irr::u32 numAnims = 0;

		Bone* bones = 0;
		irr::u32 numBones = 0;
	};

	//
	struct ComplexMeshSceneNode {
		irr::scene::IMeshSceneNode* node = 0;
        const ComplexMeshData* data = 0;

        irr::scene::ISceneNode** boneNodes;
	};

	class AssimpMeshLoader : public irr::scene::IMeshLoader {
		AssimpMeshLoader();
		virtual ~AssimpMeshLoader();

		virtual irr::scene::IAnimatedMesh* createMesh(irr::io::IReadFile* file);

		virtual bool isALoadableFileExtension(const irr::io::path& filename);

	};

	void debugAiNode(const aiScene* scene, const aiNode* node, unsigned int depth);

	void convertTransform(const aiMatrix4x4& aoffsetMatrix, irr::core::matrix4& doffsetMatrix);

	irr::u32 recursiveFindTreeSize(const aiNode* rootNode);
	void recursiveBuildBoneStructure(Bone* boneArray, irr::u32& currIndex, irr::u32 parentIndex, bool isRoot, const aiNode* copyFrom);

	ComplexMeshData* loadUsingAssimp(irr::scene::ISceneManager* smgr, std::string filename);

	ComplexMeshSceneNode* qux(irr::scene::ISceneManager* smgr, const ComplexMeshData* data, irr::gui::IGUIEnvironment* gui);

}

#endif // REIASSIMP_H
