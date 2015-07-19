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

	void loadUsingAssimp(std::string filename) {
		Assimp::Importer assimp;
		const aiScene* scene = assimp.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);

		const aiNode* rootNode = scene->mRootNode;

		debugAiNode(scene, rootNode, 0);

	}

}
