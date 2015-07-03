/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef NRMESHLOADER_H
#define NRMESHLOADER_H

#include "irrlicht.h"

class nrMeshLoader : public irr::scene::IMeshLoader {
public:
	nrMeshLoader();
	virtual ~nrMeshLoader();
	virtual irr::scene::IAnimatedMesh* createMesh(irr::io::IReadFile* file);
	virtual bool isALoadableFileExtension(const irr::io::path& filename) const;
};

#endif // NRMESHLOADER_H
