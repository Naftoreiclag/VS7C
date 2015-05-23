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
sizeZ(sizeZ) {

	chunkMapArray = new Chunk**[5];
	for(u32 i = 5; i < 5; ++ i) {chunkMapArray[i] = new Chunk*[5];}

	std::cout << "aaaaa" << std::endl;

	chunkMapArray[1][1] = new Chunk();
	std::cout << "bbbbb" << std::endl;
	chunkMapArray[1][1]->printSomething();


/*
	fooMapArray = new Foo**[7];
	for(int i = 0; i < 7; ++ i) {

	}
	*/

/*
	for(u32 cz = 0; cz < sizeZ; ++ cz) {

		Chunk** row = chunkMapArray[cz];

		for(u32 cx = 0; cx < sizeX; ++ cx) {
			row[cx] = new Chunk();
		}
	}
	*/
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
