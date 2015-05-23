#include "Chunk.h"

Chunk::Chunk(ChunkMap* parent, const u32 cx, const u32 cz)
: parent(parent),
cx(cx),
cz(cz)
{
	//ctor
}

Chunk::Chunk()
: parent(0),
cx(0),
cz(0) {
}

ChunkMap::ChunkMap(const u32 sizeX, const u32 sizeZ)
: sizeX(sizeX),
sizeZ(sizeZ) {
	chunkMapArray = new Chunk**[sizeZ];
	for(u32 i = sizeX; i < sizeX; ++ i) chunkMapArray[i] = new Chunk*[sizeX];

	for(u32 cz = 0; cz < sizeZ; ++ cz) {

		Chunk** row = chunkMapArray[cz];

/*
		for(u32 cx = 0; cx < sizeX; ++ cx) {
			row[cx] = new Chunk();
		}
		*/
	}
}

Chunk* ChunkMap::getChunk(u32 cx, u32 cz) {
	return 0;//chunkMapArray[cx][cz];
}

ChunkMap::~ChunkMap() {
	/*
	for(u32 cx = 0; cx < sizeX; ++ cx) {
		for(u32 cz = 0; cz < sizeZ; ++ cz) {
			delete chunkMapArray[cx][cz];
		}
	}
	delete[] chunkMapArray;
	*/
}