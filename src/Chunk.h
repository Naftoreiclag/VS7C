/* Copyright (c) 2015 "Naftoreiclag" https://github.com/Naftoreiclag
 *
 * Distributed under the Apache License Version 2.0 (http://www.apache.org/licenses/)
 * See accompanying file LICENSE
 */

#ifndef CHUNK_H
#define CHUNK_H

#include "irrlicht.h"
using namespace irr;

#include "Array2d.h"

/*
A 66x66 foot section of land. (Each is 1/10th of an acre)

Heightmap vertical step is 4 inches. (1/3rd foot)
Horizontal steps are 1 foot each.

"Grass" layer grid is 6x6 inches (1/2th foot)
00TTTHHH
T = type (nothing, grass, snow)
H = height (1,2,3,4,5,6,7,8 inches deep)

"River layer"?

*/
class ChunkMap;

class Chunk {
public:
	static const u32 chunkSize = 66;
	static const u32 grassSize = 2;

	Chunk(ChunkMap* parent, const u32 cx, const u32 cz);
	Array2d<u16> heightMap;
	Array2d<u8> grassMap;

	const ChunkMap* parent;
	const u32 cx;
	const u32 cz;
};

class ChunkMap {
public:
	ChunkMap(const u32 sizeX, const u32 sizeZ);
	~ChunkMap();

	Chunk* getChunk(u32 cx, u32 cz);
	u16 getHeight(u32 ax, u32 az);
	u8 getGrass(u32 ax, u32 az);

	const u32 sizeX;
	const u32 sizeZ;
private:
	Array2d<Chunk*> chunkMapArray;
};

#endif // CHUNK_H
