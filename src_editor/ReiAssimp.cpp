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
	CustomNode* loadUsingAssimp(irr::scene::ISceneManager* smgr, std::string filename) {
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

				// K for key
                for(unsigned int k = 0; k < numKeys; ++ k) {
					const aiVectorKey& key = nodeAnim->mPositionKeys[k];

					std::cout << "  ";
					std::cout << key.mTime << std::endl;

                }

            }
		}

		for(unsigned int i = 0; i < rootNode->mNumChildren; ++ i) {
			const aiNode* node = rootNode->mChildren[i];

			// If this node is the one with the mesh(es)
			if(node->mNumMeshes > 0) {

				// Begin copying this data
				CustomNode* dmesh = new CustomNode();

				dmesh->mesh = new irr::scene::SMesh();
				dmesh->numBuffers = node->mNumMeshes;
				dmesh->buffers = new ExtraBufferData[dmesh->numBuffers];

				// For every buffer
				for(unsigned int i = 0; i < node->mNumMeshes; ++ i) {

					const aiMesh* abuffer = scene->mMeshes[node->mMeshes[i]];
					irr::scene::SMeshBuffer* ibuffer = new irr::scene::SMeshBuffer();
					ExtraBufferData& dbuffer = dmesh->buffers[i];

					bool vertColors = false;
					bool texCoords = false;

					dbuffer.numVerts = abuffer->mNumVertices;
					dbuffer.verts = new ExtraVertData[dbuffer.numVerts];

					// Copy vertex data
					ibuffer->Vertices.reallocate(abuffer->mNumVertices);
					ibuffer->Vertices.set_used(abuffer->mNumVertices);
					for(unsigned int j = 0; j < abuffer->mNumVertices; ++ j) {
						irr::video::S3DVertex& ivert = ibuffer->Vertices[j];

						const aiVector3D& avert = abuffer->mVertices[j];
						const aiVector3D& anormal = abuffer->mNormals[j];
						const aiColor4D* acolor = abuffer->mColors[j];

						ivert.Pos.set(avert.x, avert.z, avert.y);
						ivert.Normal.set(anormal.x, anormal.z, anormal.y);
						ivert.Color.set(acolor->a, acolor->r, acolor->g, acolor->b);


						// Track if any vertex uses textures and or vertex colors
						vertColors = vertColors | abuffer->HasVertexColors(j);
						texCoords = texCoords | abuffer->HasTextureCoords(j);
					}

					// Bone data
					if(abuffer->HasBones()) {
						unsigned int numBones = abuffer->mNumBones;

						for(unsigned int j = 0; j < abuffer->mNumBones; ++ j) {
							aiBone* abone = abuffer->mBones[j];
							for(unsigned int k = 0; k < abone->mNumWeights; ++ k) {

								const aiVertexWeight& aweight = abone->mWeights[k];
								ExtraVertData& dvert = dbuffer.verts[aweight.mVertexId];

                                if(dvert.boneW == 255) {
									dvert.boneW = j;
									dvert.weightW = aweight.mWeight;
                                } else if(dvert.boneX == 255) {
									dvert.boneX = j;
									dvert.weightX = aweight.mWeight;
                                } else if(dvert.boneY == 255) {
									dvert.boneY = j;
									dvert.weightY = aweight.mWeight;
                                } else if(dvert.boneZ == 255) {
									dvert.boneZ = j;
									dvert.weightW = aweight.mWeight;
                                } else {
									// MORE THAN 4 BONES!!!!
                                }
							}
						}
					}

					// Copy indice data
					ibuffer->Indices.reallocate(abuffer->mNumFaces * 3);
					ibuffer->Indices.set_used(abuffer->mNumFaces * 3);
					for(unsigned int j = 0; j < abuffer->mNumFaces; ++ j) {
						const aiFace& aface = abuffer->mFaces[j];

						unsigned int A = aface.mIndices[0];
						unsigned int B = aface.mIndices[1];
						unsigned int C = aface.mIndices[2];

						ibuffer->Indices[j * 3    ] = A;
						ibuffer->Indices[j * 3 + 1] = C;
						ibuffer->Indices[j * 3 + 2] = B;
					}

					// Self-explanitory
					ibuffer->recalculateBoundingBox();

					// Copy material
					const aiMaterial* amaterial = scene->mMaterials[abuffer->mMaterialIndex];

					aiColor3D adiffuse(1.0f, 1.0f, 1.0f);
					amaterial->Get(AI_MATKEY_COLOR_DIFFUSE, adiffuse);

					irr::video::SMaterial& imaterial = ibuffer->getMaterial();
					imaterial.DiffuseColor = irr::video::SColor(255, adiffuse.r * 255.f, adiffuse.g * 255.f, adiffuse.b * 255.f);
					imaterial.AmbientColor = irr::video::SColor(255, 255, 255, 255);
					imaterial.ColorMaterial = vertColors ? 1 : 0;

					// Add da buffer
					dmesh->mesh->addMeshBuffer(ibuffer);
					ibuffer->drop();
					ibuffer = 0;
				}

				return dmesh;

				break;
			}
		}




	}

}
