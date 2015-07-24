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
				std::cout << "Node with a mesh found." << std::endl;

				// Begin copying this data
				CustomNode* output = new CustomNode();

				output->mesh = new irr::scene::SMesh();
				output->numBuffers = node->mNumMeshes;
				output->buffers = new BufferMetadata[output->numBuffers];

				std::cout << "Begin copying buffers..." << std::endl;
				for(unsigned int i = 0; i < node->mNumMeshes; ++ i) {
					std::cout << "Processing buffer " << i << std::endl;
					const aiMesh* abuffer = scene->mMeshes[node->mMeshes[i]];

					irr::scene::SMeshBuffer* ibuffer = new irr::scene::SMeshBuffer();
					BufferMetadata& dbuffer = output->buffers[i];

					bool vertColors = false;
					bool texCoords = false;
					ibuffer->Vertices.reallocate(abuffer->mNumVertices);
					ibuffer->Vertices.set_used(abuffer->mNumVertices);
					dbuffer.numVerts = abuffer->mNumVertices;
					dbuffer.verts = new VertexMetadata[dbuffer.numVerts];
					std::cout << "Begin copying vertexes..." << std::endl;
					for(unsigned int j = 0; j < abuffer->mNumVertices; ++ j) {
						const aiVector3D& avert = abuffer->mVertices[j];
						const aiVector3D& anormal = abuffer->mNormals[j];
						const aiColor4D* acolor = abuffer->mColors[j];

						irr::video::S3DVertex& ivert = ibuffer->Vertices[j];

						ivert.Pos.set(avert.x, avert.z, avert.y);
						ivert.Normal.set(anormal.x, anormal.z, anormal.y);
						//ivert.Color.set(acolor->a, acolor->r, acolor->g, acolor->b);
						ivert.Color.set(0, 0, 0, 0);

						vertColors = vertColors | abuffer->HasVertexColors(j);
						texCoords = texCoords | abuffer->HasTextureCoords(j);
					}
					std::cout << "End copying vertexes." << std::endl;

					if(abuffer->HasBones()) {
						std::cout << "Buffer has bones." << std::endl;
						dbuffer.numBones = abuffer->mNumBones;
						dbuffer.bones = new BoneData[dbuffer.numBones];

						std::cout << "Begin copying bones..." << std::endl;
						for(unsigned int j = 0; j < abuffer->mNumBones; ++ j) {
							const aiBone* abone = abuffer->mBones[j];

                            BoneData& dbone = dbuffer.bones[j];
                            dbone.boneName = abone->mName.C_Str();
                            const aiMatrix4x4& aoffsetMatrix = abone->mOffsetMatrix;
                            irr::core::matrix4& doffsetMatrix = dbone.offsetMatrix;

                            doffsetMatrix[ 0] = aoffsetMatrix.a1;
                            doffsetMatrix[ 1] = aoffsetMatrix.a2;
                            doffsetMatrix[ 2] = aoffsetMatrix.a3;
                            doffsetMatrix[ 3] = aoffsetMatrix.a4;

                            doffsetMatrix[ 4] = aoffsetMatrix.b1;
                            doffsetMatrix[ 5] = aoffsetMatrix.b2;
                            doffsetMatrix[ 6] = aoffsetMatrix.b3;
                            doffsetMatrix[ 7] = aoffsetMatrix.b4;

                            doffsetMatrix[ 8] = aoffsetMatrix.c1;
                            doffsetMatrix[ 9] = aoffsetMatrix.c2;
                            doffsetMatrix[10] = aoffsetMatrix.c3;
                            doffsetMatrix[11] = aoffsetMatrix.c4;

                            doffsetMatrix[12] = aoffsetMatrix.d1;
                            doffsetMatrix[13] = aoffsetMatrix.d2;
                            doffsetMatrix[14] = aoffsetMatrix.d3;
                            doffsetMatrix[15] = aoffsetMatrix.d4;

							std::cout << "Processing bone " << dbone.boneName << std::endl;
							for(unsigned int k = 0; k < abone->mNumWeights; ++ k) {
								const aiVertexWeight& aweight = abone->mWeights[k];
								VertexMetadata& dvert = dbuffer.verts[aweight.mVertexId];

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
									std::cout << "Warning: Vertex " << aweight.mVertexId << " has more than 4 influences." << std::endl;
                                }
							}
						}
					}

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
					output->mesh->addMeshBuffer(ibuffer);
					ibuffer->drop();
					ibuffer = 0;
				}

				return output;

				break;
			}
		}




	}

}
