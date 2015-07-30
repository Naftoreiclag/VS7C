/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "ReiAssimp.h"

#include "assimp/quaternion.h"

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

	void printThis(const irr::core::matrix4& doffsetMatrix) {

		std::cout << doffsetMatrix[ 0] << " | " << doffsetMatrix[ 1] << " | " << doffsetMatrix[ 2] << " | " << doffsetMatrix[ 3] << " | " << std::endl;
		std::cout << doffsetMatrix[ 4] << " | " << doffsetMatrix[ 5] << " | " << doffsetMatrix[ 6] << " | " << doffsetMatrix[ 7] << " | " << std::endl;
		std::cout << doffsetMatrix[ 8] << " | " << doffsetMatrix[ 9] << " | " << doffsetMatrix[10] << " | " << doffsetMatrix[11] << " | " << std::endl;
		std::cout << doffsetMatrix[12] << " | " << doffsetMatrix[13] << " | " << doffsetMatrix[14] << " | " << doffsetMatrix[15] << " | " << std::endl;
	}

	void convertTransform(const aiMatrix4x4& aoffsetMatrix, irr::core::matrix4& doffsetMatrix) {

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
	}

	irr::u32 recursiveFindTreeSize(const aiNode* rootNode) {
		irr::u32 retVal = 1;

        for(unsigned int i = 0; i < rootNode->mNumChildren; ++ i) {
			retVal += recursiveFindTreeSize(rootNode->mChildren[i]);
        }

		return retVal;
	}

	void inline printThis(irr::core::vector3df vec) {
		std::cout << "[" << vec.X << ", " << vec.Y << ", " << vec.Z << "]" << std::endl;
	}

	void recursiveBuildBoneStructure(Bone* boneArray, irr::u32& currIndex, irr::u32 parentId, bool isRoot, const aiNode* copyFrom) {

		irr::u32 myId = currIndex;
		Bone& dbone = boneArray[myId];

		dbone.parentId = parentId;
		dbone.isRoot = isRoot;

		dbone.name = copyFrom->mName.C_Str();
		dbone.numChildren = copyFrom->mNumChildren;
		//dbone.trans = copyFrom->mTransformation;
		convertTransform(copyFrom->mTransformation, dbone.trans);


		/*
		std::cout << "Translation: ";
		printThis(dbone.trans.getTranslation());
		std::cout << "Rotation: ";
		printThis(dbone.trans.getRotationDegrees());
		*/

        for(unsigned int h = 0; h < copyFrom->mNumChildren; ++ h) {
			++ currIndex;
			recursiveBuildBoneStructure(boneArray, currIndex, myId, false, copyFrom->mChildren[h]);
        }
	}

	/*
	Converting from dae:
	model.x <- -dae.y
	model.y <- dae.z
	model.z <- dae.x

	 0 -1  0  0
	 0  0  1  0
	 1  0  0  0
	 0  0  0  1

	 0  1  2  3
	 4  5  6  7
	 8  9 10 11
	12 13 14 15

	*/

	ComplexMeshData* loadUsingAssimp(irr::scene::ISceneManager* smgr, std::string filename) {

		irr::core::matrix4 modification;
		for(int i = 0; i < 16; ++ i) {
			modification[i] = 0;
		}

		modification[1] = -1;
		modification[6] = 1;
		modification[8] = 1;
		modification[15] = 1;

		modification.makeIdentity();

		Assimp::Importer assimp;
		const aiScene* ascene = assimp.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);

		const aiNode* rootNode = ascene->mRootNode;

		debugAiNode(ascene, rootNode, 0);

		unsigned int numAnim = ascene->mNumAnimations;

		std::cout << "numAnim = " << numAnim << std::endl;

		for(int i = 0; i < numAnim; ++ i) {
			aiAnimation* anim = ascene->mAnimations[i];

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

		for(unsigned int h = 0; h < rootNode->mNumChildren; ++ h) {
			const aiNode* node = rootNode->mChildren[h];

			// If this node is the one with the mesh(es)
			if(node->mNumMeshes > 0) {
				std::cout << "Node with a mesh found." << std::endl;

				// Begin copying this data
				ComplexMeshData* output = new ComplexMeshData();

				// Copying bone structure
				std::cout << "Begin copying bone structure..." << std::endl;
				const aiNode* armatureRoot = rootNode->FindNode("ROOT");
				if(armatureRoot) {
                    output->numBones = recursiveFindTreeSize(armatureRoot);
					std::cout << "Total bone count: " << output->numBones << std::endl;

                    if(output->numBones > 0) {
						output->bones = new Bone[output->numBones];

						irr::u32 persistentIndex = 0;
						std::cout << "Generating bone structure." << std::endl;
						recursiveBuildBoneStructure(output->bones, persistentIndex, 0, true, armatureRoot);
                    }
                    for(unsigned int i = 0; i < output->numBones; ++ i) {
						Bone& boney = output->bones[i];
						boney.trans = boney.trans * modification;
                    }
				}
				std::cout << "End copying bone structure." << std::endl;

				output->mesh = new irr::scene::SMesh();
				output->numBuffers = node->mNumMeshes;
				output->buffers = new BufferMetadata[output->numBuffers];

				std::cout << "Begin copying buffers..." << std::endl;
				std::cout << "Buffer count " << node->mNumMeshes << std::endl;
				for(unsigned int i = 0; i < node->mNumMeshes; ++ i) {
					std::cout << "Buffer #" << i << std::endl;
					const aiMesh* abuffer = ascene->mMeshes[node->mMeshes[i]];

					irr::scene::SMeshBuffer* ibuffer = new irr::scene::SMeshBuffer();
					BufferMetadata& dbuffer = output->buffers[i];

					bool vertColors = false;
					bool texCoords = false;
					ibuffer->Vertices.reallocate(abuffer->mNumVertices);
					ibuffer->Vertices.set_used(abuffer->mNumVertices);
					dbuffer.numVerts = abuffer->mNumVertices;
					dbuffer.verts = new VertexMetadata[dbuffer.numVerts];
					std::cout << "Begin copying vertexes..." << std::endl;
					std::cout << "Vertex count " << abuffer->mNumVertices << std::endl;
					for(unsigned int j = 0; j < abuffer->mNumVertices; ++ j) {
						const aiVector3D& avert = abuffer->mVertices[j];
						const aiVector3D& anormal = abuffer->mNormals[j];
						const aiColor4D* acolor = abuffer->mColors[j];

						irr::video::S3DVertex& ivert = ibuffer->Vertices[j];

						ivert.Pos.set(avert.x, avert.y, avert.z);
						ivert.Normal.set(anormal.x, anormal.y, anormal.z);
						//ivert.Color.set(acolor->a, acolor->r, acolor->g, acolor->b);
						ivert.Color.set(0, 0, 0, 0);

						vertColors = vertColors | abuffer->HasVertexColors(j);
						texCoords = texCoords | abuffer->HasTextureCoords(j);
					}
					std::cout << "End copying vertexes." << std::endl;

					if(abuffer->HasBones()) {
						dbuffer.numBones = abuffer->mNumBones;
						dbuffer.usedBones = new BoneMetadata[dbuffer.numBones];

						std::cout << "Begin copying buffer bone groups..." << std::endl;
						std::cout << "Bones used " << abuffer->mNumBones << std::endl;
						for(unsigned int j = 0; j < abuffer->mNumBones; ++ j) {
							std::cout << "Bone #" << j << std::endl;
							const aiBone* abone = abuffer->mBones[j];

                            BoneMetadata& dbonem = dbuffer.usedBones[j];
                            std::string boneName = abone->mName.C_Str();
							std::cout << "Processing bone " << boneName << std::endl;

                            for(unsigned int k = 0; k < output->numBones; ++ k) {
								Bone& dbone = output->bones[k];

								if(dbone.name == boneName) {
									std::cout << "Match found" << std::endl;
									dbonem.boneId = k;


									//dbone.offsetMatrix = abone->mOffsetMatrix;
									convertTransform(abone->mOffsetMatrix, dbone.offsetMatrix);
									dbone.offsetMatrix = dbone.offsetMatrix * modification;

								}
                            }

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

						std::cout << "End copying buffer bone groups." << std::endl;
					}


					ibuffer->Indices.reallocate(abuffer->mNumFaces * 3);
					ibuffer->Indices.set_used(abuffer->mNumFaces * 3);
					std::cout << "Begin copying triangles..." << std::endl;
					std::cout << "Triangle count " << abuffer->mNumFaces << std::endl;
					for(unsigned int j = 0; j < abuffer->mNumFaces; ++ j) {

						const aiFace& aface = abuffer->mFaces[j];

						unsigned int A = aface.mIndices[0];
						unsigned int B = aface.mIndices[1];
						unsigned int C = aface.mIndices[2];

						ibuffer->Indices[j * 3    ] = A;
						ibuffer->Indices[j * 3 + 1] = B;
						ibuffer->Indices[j * 3 + 2] = C;
					}
					std::cout << "End copying triangles..." << std::endl;

					std::cout << "Recalculating bounding box..." << std::endl;
					ibuffer->recalculateBoundingBox();

					// Copy material
					const aiMaterial* amaterial = ascene->mMaterials[abuffer->mMaterialIndex];

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

				// Copying Animations
				std::cout << "Begin copying animation data..." << std::endl;
				output->numAnims = ascene->mNumAnimations;
				output->anims = new AnimationData[output->numAnims];
				std::cout << "Animation count: " << ascene->mNumAnimations << std::endl;
				for(unsigned int i = 0; i < ascene->mNumAnimations; ++ i) {
					std::cout << "Animation #" << i << std::endl;
                    const aiAnimation* aanim = ascene->mAnimations[i];
                    AnimationData& danim = output->anims[i];

					std::cout << "Begin copying channel data..." << std::endl;
					danim.numChannels = aanim->mNumChannels;
					danim.channels = new ChannelData[danim.numChannels];
					std::cout << "Channel count: " << aanim->mNumChannels << std::endl;
                    for(unsigned int j = 0; j < aanim->mNumChannels; ++ j) {
						std::cout << "Channel #" << j << std::endl;
                        const aiNodeAnim* achannel = aanim->mChannels[j];
                        ChannelData& dchannel = danim.channels[j];

						dchannel.boneName = achannel->mNodeName.C_Str();

						std::cout << "Begin copying position keys..." << std::endl;
						dchannel.numPositions = achannel->mNumPositionKeys;
						dchannel.positions = new VectorKey[dchannel.numPositions];
                        for(unsigned int k = 0; k < achannel->mNumPositionKeys; ++ k) {
							aiVectorKey& akey = achannel->mPositionKeys[k];
                            VectorKey& dkey = dchannel.positions[k];

							aiVector3D& avalue = akey.mValue;

							dkey.time = akey.mTime;
							dkey.value = irr::core::vector3df(-avalue.y, avalue.z, avalue.x);
                        }
						std::cout << "End copying position keys." << std::endl;

						std::cout << "Begin copying rotation keys..." << std::endl;
						dchannel.numRotations = achannel->mNumRotationKeys;
						dchannel.rotations = new QuaternionKey[dchannel.numRotations];
                        for(unsigned int k = 0; k < achannel->mNumRotationKeys; ++ k) {
							aiQuatKey& akey = achannel->mRotationKeys[k];
                            QuaternionKey& dkey = dchannel.rotations[k];

							aiQuaternion& avalue = akey.mValue;

							dkey.time = akey.mTime;
							dkey.value.set(-avalue.y, avalue.z, avalue.x, -avalue.w);
                        }
						std::cout << "End copying rotation keys." << std::endl;

						std::cout << "Begin copying scale keys..." << std::endl;
						dchannel.numScalings = achannel->mNumScalingKeys;
						dchannel.scalings = new VectorKey[dchannel.numScalings];
                        for(unsigned int k = 0; k < achannel->mNumScalingKeys; ++ k) {
							aiVectorKey& akey = achannel->mScalingKeys[k];
                            VectorKey& dkey = dchannel.scalings[k];

							aiVector3D& avalue = akey.mValue;

							dkey.time = akey.mTime;
							dkey.value = irr::core::vector3df(avalue.x, avalue.y, avalue.z);
                        }
						std::cout << "End copying scale keys." << std::endl;
                    }
					std::cout << "End copying channel data." << std::endl;
				}
				std::cout << "End copying animation data." << std::endl;

				//assimp.FreeScene();

				return output;
			}
		}

		return 0;
	}

	const wchar_t* toText(std::string value) {
		std::wstring ws(value.begin(), value.end());
		return ws.c_str();
	}

	ComplexMeshSceneNode* addNodeFromMesh(irr::scene::ISceneManager* smgr, const ComplexMeshData* data, irr::gui::IGUIFont* fnt, irr::scene::IAnimatedMesh* boneThing) {
		ComplexMeshSceneNode* retVal = new ComplexMeshSceneNode();
		retVal->node = smgr->addMeshSceneNode(data->mesh);
		retVal->data = data;

		retVal->boneNodes = new irr::scene::ISceneNode*[data->numBones];
		for(int i = 0; i < data->numBones; ++ i) {
			const Bone& bone = data->bones[i];


			if(bone.isRoot) {
				retVal->boneNodes[i] = smgr->addEmptySceneNode(retVal->node);

			}
			else {
				retVal->boneNodes[i] = smgr->addEmptySceneNode(retVal->boneNodes[bone.parentId]);
			}

			if(boneThing) {
				irr::scene::IAnimatedMeshSceneNode* armatureDecor = smgr->addAnimatedMeshSceneNode(boneThing, retVal->boneNodes[i]);
				irr::scene::ISceneNode* text = smgr->addTextSceneNode(fnt, toText(bone.name), irr::video::SColor(255, 255, 255, 255), retVal->boneNodes[i]);

				text->setPosition(irr::core::vector3df(0.f, 0.5f, 0.f));
			}

			irr::core::matrix4 finTrans = bone.trans;// * bone.offsetMatrix;

			retVal->boneNodes[i]->setPosition(finTrans.getTranslation());
			retVal->boneNodes[i]->setRotation(finTrans.getRotationDegrees());
			retVal->boneNodes[i]->setScale(finTrans.getScale());
		}

		return retVal;

	}

	void poseNode(ComplexMeshSceneNode* node, irr::f32 tNow) {

		//node->node->setRotation(irr::core::vector3df(-90.f, 0.f, 0.f));

		const ComplexMeshData* data = node->data;

		AnimationData& anim = data->anims[0];

		for(unsigned int i = 0; i < anim.numChannels; ++ i) {
			ChannelData& channel = anim.channels[i];

			irr::core::vector3df timePosition(0.f, 0.f, 0.f);
			irr::core::quaternion timeRotations(0.f, 0.f, 0.f, 0.f);
			irr::core::vector3df timeScale(1.f, 1.f, 1.f);

			if(channel.numPositions == 1) {
				timePosition = channel.positions[0].value;
			}
			else {
                if(tNow < channel.positions[0].time) {
                    timePosition = channel.positions[0].value;
                }
                else if(channel.positions[channel.numPositions - 1].time < tNow) {
					timePosition = channel.positions[channel.numPositions - 1].value;
                }
                else {
					for(unsigned int j = 1; j < channel.numPositions; ++ j) {
						if(tNow < channel.positions[j].time) {
							// The key to the left (Backward in time)
							VectorKey& before = channel.positions[j - 1];
							// The key to the right (Forward in time)
							VectorKey& after = channel.positions[j];
							// How close the current time is to the "after" time (Expressed as a fraction of the total time between those keyframes)
							irr::f32 progress = (tNow - before.time) / (after.time - before.time);

							irr::core::vector3df displ = after.value - before.value;
                            timePosition = before.value + (displ * progress);
						}
					}
                }
			}

			if(channel.numRotations == 1) {
				timeRotations = channel.rotations[0].value;
			}
			else {
                if(tNow < channel.rotations[0].time) {
                    timeRotations = channel.rotations[0].value;
                }
                else if(channel.rotations[channel.numRotations - 1].time < tNow) {
					timeRotations = channel.rotations[channel.numRotations - 1].value;
                }
                else {
					for(unsigned int j = 1; j < channel.numRotations; ++ j) {
						if(tNow < channel.rotations[j].time) {
							QuaternionKey& before = channel.rotations[j - 1];
							QuaternionKey& after = channel.rotations[j];
							irr::f32 progress = (tNow - before.time) / (after.time - before.time);

							timeRotations.slerp(before.value, after.value, progress);
						}
					}
                }
			}

			if(channel.numScalings == 1) {
				timeScale = channel.scalings[0].value;
			}
			else {
                if(tNow < channel.scalings[0].time) {
                    timeScale = channel.scalings[0].value;
                }
                else if(channel.scalings[channel.numScalings - 1].time < tNow) {
					timeScale = channel.scalings[channel.numScalings - 1].value;
                }
                else {
					for(unsigned int j = 1; j < channel.numScalings; ++ j) {
						if(tNow < channel.scalings[j].time) {
							VectorKey& before = channel.scalings[j - 1];
							VectorKey& after = channel.scalings[j];
							irr::f32 progress = (tNow - before.time) / (after.time - before.time);

							irr::core::vector3df displ = after.value - before.value;
                            timeScale = before.value + (displ * progress);
						}
					}
                }
			}

			const std::string& boneName = channel.boneName;

			for(unsigned int j = 0; j < data->numBones; ++ j) {
				Bone& bone = data->bones[j];

				if(bone.name == boneName) {
					irr::scene::ISceneNode* boneNode = node->boneNodes[j];

					boneNode->setPosition(timePosition);

					irr::core::vector3df rotation;
					timeRotations.toEuler(rotation);
					rotation *= 180.f / 3.14159265f;
					boneNode->setRotation(rotation);

					boneNode->setScale(timeScale);
				}
			}
		}
	}
}
