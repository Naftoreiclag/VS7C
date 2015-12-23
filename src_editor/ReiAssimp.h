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
        irr::u8 boneIds[4];
        irr::f32 weights[4];
        VertexMetadata() {
            boneIds[0] = 255;
            boneIds[1] = 255;
            boneIds[2] = 255;
            boneIds[3] = 255;
        }
    };

    // Extra data for each buffer (group of vertices that has the same material or otherwise differentiated from the rest of the mesh)
    struct BufferMetadata {
        // The metadata to apply to each vertex
        VertexMetadata* verts = 0;
        irr::u32 numVerts;

        // Describes what the vertex data holds
        bool usePos = true;
        bool useNorm = true;
        bool useBone = false;
        bool useColor = false;
        bool useTexture = false;

        irr::u8 materialIndex = 255;

        ~BufferMetadata() {
            delete[] verts;
        }
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
        irr::u32 boneId;

        VectorKey* positions;
        irr::u32 numPositions = 0;

        QuaternionKey* rotations;
        irr::u32 numRotations = 0;

        VectorKey* scalings;
        irr::u32 numScalings = 0;

        ~ChannelData() {
            delete[] positions;
            delete[] rotations;
            delete[] scalings;
        }
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

        // CMatrix4< f32 >
        irr::core::matrix4 inverseBindPose;
        irr::core::matrix4 bindPose;
    };

    struct MaterialMetadata {
        std::string name;

        bool hasDiffuseTexture = false;
        std::string diffuseTexturePath;
    };

    // A single mesh with some number of bones and animations
    struct ComplexMeshData {
        irr::scene::SMesh* mesh = 0;

        MaterialMetadata* materials = 0;
        irr::u32 numMaterials = 0;

        BufferMetadata* buffers = 0;
        irr::u32 numBuffers = 0;

        AnimationData* anims = 0;
        irr::u32 numAnims = 0;

        Bone* bones = 0;
        irr::u32 numBones = 0;

        ~ComplexMeshData() {
            delete mesh;
            delete[] buffers;
            delete[] anims;
            delete[] bones;
        }
    };

    //
    struct ComplexMeshSceneNode {
        irr::scene::IMeshSceneNode* node = 0;
        const ComplexMeshData* data = 0;
        irr::scene::SMesh* instancedMesh = 0;

        irr::scene::ISceneNode** boneNodes;

        ~ComplexMeshSceneNode() {
            node->remove();
            delete instancedMesh;
        }
    };

    class AssimpMeshLoader : public irr::scene::IMeshLoader {
        AssimpMeshLoader();
        virtual ~AssimpMeshLoader();

        virtual irr::scene::IAnimatedMesh* createMesh(irr::io::IReadFile* file);

        virtual bool isALoadableFileExtension(const irr::io::path& filename);

    };

    irr::scene::SMesh* meshClone(const irr::scene::SMesh* mesh);

    void debugAiNode(const aiScene* scene, const aiNode* node, irr::u32 depth);

    void convertTransform(const aiMatrix4x4& aoffsetMatrix, irr::core::matrix4& doffsetMatrix);

    // Finds how large an aiNode tree is
    irr::u32 recursiveFindTreeSize(const aiNode* rootNode);

    // Figure out the bone structure recursively
    void recursiveBuildBoneStructure(Bone* boneArray, irr::u32& currIndex, irr::u32 parentIndex, bool isRoot, const aiNode* copyFrom);

    // Load using assimp
    ComplexMeshData* loadUsingAssimp(irr::scene::ISceneManager* smgr, std::string filename);

    // Build the scenenode structure for the given mesh data
    ComplexMeshSceneNode* addNodeFromMesh(irr::scene::ISceneManager* smgr, ComplexMeshData* data, irr::gui::IGUIFont* fnt = 0, irr::scene::IAnimatedMesh* boneThing = 0);

    // Set all the relative transforms of the bone nodes
    void poseBones(ComplexMeshSceneNode* node, irr::f32 time);

    // Modify the vertex buffers of a node given the absolute transformations of the bones
    void poseNode(ComplexMeshSceneNode* node, irr::f32 time);

}

#endif // REIASSIMP_H
