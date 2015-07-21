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

				/*
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


				if(material->mNumProperties > 0) {

					std::cout << ":[";


					for(unsigned int j = 0; j < material->mNumProperties; ++ j) {
						aiMaterialProperty* property = material->mProperties[j];

						aiPropertyTypeInfo* info = property->mType;



						std::cout << property->mKey.C_Str() << "=" << property->mData << std::endl;
					}
					std::cout << "]";

				}
				*/

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
	irr::scene::SMeshBuffer* loadUsingAssimp(irr::scene::ISceneManager* smgr, std::string filename) {
		Assimp::Importer assimp;
		const aiScene* scene = assimp.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);

		const aiNode* rootNode = scene->mRootNode;

		debugAiNode(scene, rootNode, 0);

		unsigned int numAnim = scene->mNumAnimations;

		std::cout << "numAnim = " << numAnim << std::endl;

		for(int i = 0; i < numAnim; ++ i) {
			aiAnimation* anim = scene->mAnimations[i];

			std::cout << "anim = " << anim->mName.C_Str() << std::endl;

			unsigned int numChannels = anim->mNumChannels;

            for(unsigned int j = 0; j < numChannels; ++ j) {

                aiNodeAnim* nodeAnim = anim->mChannels[j];
                std::cout << "Channel " << j << " ";
                std::cout << nodeAnim->mNodeName.C_Str();
                std::cout << std::endl;

                unsigned int numKeys = nodeAnim->mNumPositionKeys;

                for(unsigned int k = 0; k < numKeys; ++ k) {
					const aiVectorKey& key = nodeAnim->mPositionKeys[k];

					std::cout << "  ";
					std::cout << key.mTime << std::endl;

                }

            }
		}

		for(unsigned int i = 0; i < rootNode->mNumChildren; ++ i) {
			const aiNode* node = rootNode->mChildren[i];

			if(node->mNumMeshes > 0) {
				irr::scene::SMesh* imesh = new irr::scene::SMesh();

				irr::scene::SMeshBuffer* buffer;
				for(unsigned int i = 0; i < node->mNumMeshes; ++ i) {

					const aiMesh* amesh = scene->mMeshes[node->mMeshes[i]];

					buffer = new irr::scene::SMeshBuffer();

					bool vertColors = false;
					bool texCoords = false;

					buffer->Vertices.reallocate(amesh->mNumVertices);
					buffer->Vertices.set_used(amesh->mNumVertices);
					buffer->Vertices.set_used(amesh->mNumVertices);

					for(int i = 0; i < amesh->mNumVertices; ++ i) {
						irr::video::S3DVertex& ivert = buffer->Vertices[i];
						const aiVector3D& avert = amesh->mVertices[i];
						const aiVector3D& anormal = amesh->mNormals[i];

						ivert.Pos.set(avert.x, avert.z, avert.y);
						ivert.Normal.set(anormal.x, anormal.z, anormal.y);
						ivert.Color.set(0, 0, 0, 0);

						vertColors = vertColors | amesh->HasVertexColors(i);
						texCoords = texCoords | amesh->HasTextureCoords(i);

					}

					buffer->Indices.reallocate(amesh->mNumFaces * 3);
					buffer->Indices.set_used(amesh->mNumFaces * 3);

					for(int i = 0; i < amesh->mNumFaces; ++ i) {
						const aiFace& aface = amesh->mFaces[i];

						unsigned int A = aface.mIndices[0];
						unsigned int B = aface.mIndices[1];
						unsigned int C = aface.mIndices[2];

						buffer->Indices[i * 3    ] = A;
						buffer->Indices[i * 3 + 1] = C;
						buffer->Indices[i * 3 + 2] = B;
					}

					buffer->recalculateBoundingBox();

					irr::video::SMaterial& imaterial = buffer->getMaterial();
					aiMaterial* amaterial = scene->mMaterials[amesh->mMaterialIndex];

					aiColor3D adiffuse(1.0f, 1.0f, 1.0f);
					amaterial->Get(AI_MATKEY_COLOR_DIFFUSE, adiffuse);

					std::cout << adiffuse.r << ", " << adiffuse.g << ", " << adiffuse.b << std::endl;

					imaterial.DiffuseColor = irr::video::SColor(255, adiffuse.r * 255.f, adiffuse.g * 255.f, adiffuse.b * 255.f);
					imaterial.AmbientColor = irr::video::SColor(255, 255, 255, 255);

					imaterial.ColorMaterial = vertColors ? 1 : 0;


					imesh->addMeshBuffer(buffer);
					buffer->drop();
				}

				irr::scene::IMeshSceneNode* node = smgr->addMeshSceneNode(imesh);

				//node->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
				node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);

				return buffer;

				break;
			}
		}




	}

}
