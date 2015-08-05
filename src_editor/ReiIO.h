/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef REIIO_H
#define REIIO_H

#include <string>
#include "ReiAssimp.h"

namespace ReiIO {

	void writeToFile(std::string filename, const reia::ComplexMeshData& data);
}

#endif // REIIO_H
