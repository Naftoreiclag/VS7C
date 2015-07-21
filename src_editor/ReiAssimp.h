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

	class AssimpMeshLoader : public irr::scene::IMeshLoader {
		AssimpMeshLoader();
		virtual ~AssimpMeshLoader();

		virtual irr::scene::IAnimatedMesh* createMesh(irr::io::IReadFile* file);

		virtual bool isALoadableFileExtension(const irr::io::path& filename);

	};

	void debugAiNode(const aiScene* scene, const aiNode* node, unsigned int depth);

	irr::scene::SMeshBuffer* loadUsingAssimp(irr::scene::ISceneManager* smgr, std::string filename);

}

#endif // REIASSIMP_H
