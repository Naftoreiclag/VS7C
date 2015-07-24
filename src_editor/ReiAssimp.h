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
	struct VertexMetadata {
		irr::u8 boneW = 255;
		irr::u8 boneX = 255;
		irr::u8 boneY = 255;
		irr::u8 boneZ = 255;

		irr::f32 weightW;
		irr::f32 weightX;
		irr::f32 weightY;
		irr::f32 weightZ;
	};
	struct BoneData {
		std::string boneName;
		irr::core::matrix4 offsetMatrix;
	};
	struct BufferMetadata {
		VertexMetadata* verts = 0;
		irr::u32 numVerts;

		BoneData* bones = 0;
		irr::u32 numBones;
	};
	struct ChannelData {
		std::string boneName;

        irr::core::vector3df positions[];
        irr::u32 numPositions;

        irr::core::quaternion rotations[];
        irr::u32 numRotations;

        irr::core::vector3df scalings[];
        irr::u32 numScalings;
	};
	struct AnimationData {
		std::string animName;
		irr::f32 duration;
		irr::f32 tps;

		ChannelData* channels;
		irr::u32 numChannels;
	};
	struct ComplexMeshData {
		irr::scene::SMesh* mesh = 0;

		BufferMetadata* buffers = 0;
		irr::u32 numBuffers;
	};

	class AssimpMeshLoader : public irr::scene::IMeshLoader {
		AssimpMeshLoader();
		virtual ~AssimpMeshLoader();

		virtual irr::scene::IAnimatedMesh* createMesh(irr::io::IReadFile* file);

		virtual bool isALoadableFileExtension(const irr::io::path& filename);

	};

	void debugAiNode(const aiScene* scene, const aiNode* node, unsigned int depth);

	ComplexMeshData* loadUsingAssimp(irr::scene::ISceneManager* smgr, std::string filename);

}

#endif // REIASSIMP_H
