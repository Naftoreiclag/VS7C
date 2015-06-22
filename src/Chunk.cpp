/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#include "Chunk.h"
#include <iostream>

Chunk::Chunk(ChunkMap* parent, const u32 cx, const u32 cz)
: parent(parent),
cx(cx),
cz(cz),
heightMap(chunkSize),
grassMap(chunkSize * grassSize) {
	//ctor
}

ChunkMap::ChunkMap(const u32 sizeX, const u32 sizeZ)
: sizeX(sizeX),
sizeZ(sizeZ),
chunkMapArray(sizeX, sizeZ) {
	for(u32 cx = 0; cx < sizeX; ++ cx) {
		for(u32 cz = 0; cz < sizeZ; ++ cz) {
			chunkMapArray(cx, cz) = new Chunk(this, cx, cz);
		}
	}
}

Chunk* ChunkMap::getChunk(u32 cx, u32 cz) {
	return chunkMapArray(cx, cz);
}

ChunkMap::~ChunkMap() {
	for(u32 cx = 0; cx < sizeX; ++ cx) {
		for(u32 cz = 0; cz < sizeZ; ++ cz) {
			delete chunkMapArray(cx, cz);
		}
	}
}
