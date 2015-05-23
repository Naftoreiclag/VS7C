#ifndef ARRAY2D_H
#define ARRAY2D_H

#include "irrlicht.h"
using namespace irr;

template <typename T>
class Array2d {
public:
	const u32 sizeX;
	const u32 sizeZ;

	Array2d(const u32 sizeX, const u32 sizeZ)
	: sizeX(sizeX),
	sizeZ(sizeZ) {
		data = new T[sizeX * sizeZ];
	};

	// Square arrays
	Array2d(const u32 squareSize)
	: sizeX(squareSize),
	sizeZ(squareSize) {
		data = new T[squareSize * squareSize];
	};

	~Array2d() {
		delete[] data;
	};

	const T& operator()(const u32 x, const u32 z) const {
        return data[z * sizeX + x];
	}
	T& operator()(const u32 x, const u32 z) {
        return data[z * sizeX + x];
	}
private:
	T* data;
};

#endif // ARRAY2D_H
