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

	fooMapArray = new Foo**[5];
	for(u32 i = 5; i < 5; ++ i) {fooMapArray[i] = new Foo*[5];}

	std::cout << "aaaaa" << std::endl;

	for(u32 cz = 0; cz < 5; ++ cz) {
		for(u32 cx = 0; cx < 5; ++ cx) {
			fooMapArray[cx][cz] = new Foo(cx * 100 + cz);
			std::cout << fooMapArray[cx][cz]->getBar() << std::endl;
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
