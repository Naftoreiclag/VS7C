/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "ReiAssimp.h"

namespace reia {

	void debugAiNode(const aiScene* scene, const aiNode* node, unsigned int depth) {

		for(int c = 0; c < depth; ++ c) {
            std::cout << "  ";
		}

        std::cout << node->mName.C_Str();

        unsigned int numMeshes = node->mNumMeshes;

        if(numMeshes > 0) {
			std::cout << " ";
			std::cout << numMeshes;
			std::cout << ":[";
			for(unsigned int i = 0; i < numMeshes; ++ i) {

				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

				std::cout << mesh->mName.C_Str();

				if(i != numMeshes - 1) {
                    std::cout << ", ";
				}
			}
			std::cout << "]";

        }
        std::cout << std::endl;

        unsigned int numChildren = node->mNumChildren;

        for(unsigned int i = 0; i < numChildren; ++ i) {
			debugAiNode(scene, node->mChildren[i], depth + 1);

        }

	}
	irr::scene::IMeshSceneNode* loadUsingAssimp(irr::scene::ISceneManager* smgr, std::string filename) {
		Assimp::Importer assimp;
		const aiScene* scene = assimp.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);

		const aiNode* rootNode = scene->mRootNode;

		for(unsigned int i = 0; i < rootNode->mNumChildren; ++ i) {
			const aiNode* node = rootNode->mChildren[i];

			if(node->mNumMeshes > 0) {
				irr::scene::SMesh* imesh = new irr::scene::SMesh();
				for(unsigned int i = 0; i < node->mNumMeshes; ++ i) {

					const aiMesh* amesh = scene->mMeshes[node->mMeshes[i]];

					irr::scene::SMeshBuffer* buffer = new irr::scene::SMeshBuffer();

					buffer->Vertices.reallocate(amesh->mNumVertices);
					buffer->Vertices.set_used(amesh->mNumVertices);

					for(int i = 0; i < amesh->mNumVertices; ++ i) {
						irr::video::S3DVertex& ivert = buffer->Vertices[i];
						const aiVector3D& avert = amesh->mVertices[i];

						ivert.Pos.set(avert.x, avert.y, avert.z);
					}

					buffer->Indices.reallocate(amesh->mNumFaces * 3);
					buffer->Indices.set_used(amesh->mNumFaces * 3);

					for(int i = 0; i < amesh->mNumFaces; ++ i) {
						const aiFace& aface = amesh->mFaces[i];

						unsigned int A = aface.mIndices[0];
						unsigned int B = aface.mIndices[1];
						unsigned int C = aface.mIndices[2];

						buffer->Indices[i * 3    ] = A;
						buffer->Indices[i * 3 + 1] = B;
						buffer->Indices[i * 3 + 2] = C;
					}

					buffer->recalculateBoundingBox();

					imesh->addMeshBuffer(buffer);
					buffer->drop();
					buffer = 0;
				}

				irr::scene::IMeshSceneNode* node = smgr->addMeshSceneNode(imesh);

				irr::video::SMaterial& mat = node->getMaterial(0);
				mat.Wireframe = true;

				node->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
				node->setMaterialFlag(irr::video::EMF_LIGHTING, false);


				break;
			}
		}


		debugAiNode(scene, rootNode, 0);

		unsigned int numAnim = scene->mNumAnimations;

		std::cout << "numAnim = " << numAnim << std::endl;

		for(int i = 0; i < numAnim; ++ i) {
			aiAnimation* anim = scene->mAnimations[i];

			std::cout << "anim = " << anim->mName.C_Str() << std::endl;

			unsigned int numChannels = anim->mNumChannels;

            for(int j = 0; j < numChannels; ++ j) {

                aiNodeAnim* nodeAnim = anim->mChannels[i];
                std::cout << "Channel " << j << " ";
                std::cout << nodeAnim->mNodeName.C_Str();
                std::cout << std::endl;

            }
		}

	}

}
