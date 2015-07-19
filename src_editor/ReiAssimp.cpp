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

	irr::scene::IAnimatedMeshSceneNode* loadUsingAssimp(irr::scene::ISceneManager* smgr, std::string filename) {
		Assimp::Importer assimp;
		const aiScene* scene = assimp.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);

		const aiNode* rootNode = scene->mRootNode;

		//smgr->addAnimatedMeshSceneNode();

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
