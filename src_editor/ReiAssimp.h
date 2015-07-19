/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef REIASSIMP_H
#define REIASSIMP_H

#include <string>
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace reia {

	void loadUsingAssimp(std::string filename) {
		Assimp::Importer assimp;
		const aiScene* scene = assimp.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);

	}
}

#endif // REIASSIMP_H
