/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "ReiAssimp.h"

namespace reia {

	void debugAiNode(const aiNode* rootNode, unsigned int depth) {

		for(int c = 0; c < depth; ++ c) {
            std::cout << "  ";
		}

        std::cout << rootNode->mName.C_Str() << std::endl;

        unsigned int numChildren = rootNode->mNumChildren;

        aiNode** children = rootNode->mChildren;
        //rootNode->mChildren.

        for(unsigned int i = 0; i < numChildren; ++ i) {
			debugAiNode(children[i], depth + 1);

        }

	}

	void loadUsingAssimp(std::string filename) {
		Assimp::Importer assimp;
		const aiScene* scene = assimp.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);

		const aiNode* rootNode = scene->mRootNode;

		debugAiNode(rootNode, 0);

	}

}
