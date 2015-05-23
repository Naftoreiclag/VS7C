#ifndef CHUNK_H
#define CHUNK_H

#include "irrlicht.h"
using namespace irr;

/*
A 66x66 foot section of land. (Each is 1/10th of an acre)

Heightmap vertical step is 4 inches. (1/3rd foot)
Horizontal steps are 1 foot each.

"Grass" layer grid is 3x3 inches (1/4th foot)
00TTTHHH
T = type (nothing, grass, snow)
H = height (1,2,3,4,5,6,7,8 inches deep)

"River layer"?

*/
class ChunkMap;

class Chunk {
public:
	Chunk(ChunkMap* parent, const u32 cx, const u32 cz);
	Chunk();
	u16 heightMap[66][66];
	u8 grass[66][66][4][4];
	const u32 cx;
	const u32 cz;
	const ChunkMap* parent;
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
	Chunk** chunkMapArray; // Chunk map array
};

#endif // CHUNK_H
