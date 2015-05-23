#include "Chunk.h"
#include <iostream>
/*
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
	std::cout << "hello" << std::endl;
}
*/

Chunk::Chunk() {
}

void Chunk::printSomething() {

	std::cout << "something" << std::endl;

}


ChunkMap::ChunkMap(const u32 sizeX, const u32 sizeZ)
: sizeX(sizeX),
sizeZ(sizeZ),
chunkMapArray(sizeX, sizeZ) {

	for(u32 cz = 0; cz < 5; ++ cz) {
		for(u32 cx = 0; cx < 5; ++ cx) {
			chunkMapArray(cx, cz) = new Chunk();
		}
	}
	std::cout << "abbbbbb" << std::endl;
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
