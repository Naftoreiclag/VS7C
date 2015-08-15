/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef REIIO_H
#define REIIO_H

#include <fstream>
#include <string>
#include "ReiAssimp.h"
#include "ProjectCompiler.h"

namespace ReiIO {

    void saveMaterial(std::string filename, const reia::MaterialMetadata& data);
	void saveComplexMesh(std::string filename, const reia::ComplexMeshData& data);
	void savePhysics(std::string filename, const ProjectCompiler::FPhysics& data);
}

#endif // REIIO_H
