#ifndef PROJECTCOMPILER_H
#define PROJECTCOMPILER_H

#include "irrlicht.h"
#include "btBulletCollisionCommon.h"

#include <string>
#include <vector>

namespace ProjectCompiler {

typedef std::string Path;
typedef double btDouble;

struct Referen {
    Path absPath;
    std::string qualifiedName;

    Referen();

    Referen(Path path, Path context);

    Referen(Path path);
};

enum PhysicsType {
	EMPTY = 0,
    SPHERE = 1,
    BOX = 2,
    CYLINDER = 3,
    CAPSULE = 4,
    CONE = 5,
    MULTI_SPHERE = 6,
    CONCAVE_TRIANGLE_MESH = 7,
    ARBITRARY_TRIANGLE_MESH = 8,
};

struct FPhysics {
	PhysicsType type = EMPTY;

	btDouble radius = 0;
	btVector3 dimensions = btVector3(0, 0, 0);
	btDouble height = 0;

	std::vector<std::pair<btVector3, btDouble>> locRadi; // MULTI_SPHERE
	btTriangleMesh* triangles = 0; // ARBITRARY_TRIANGLE_MESH
};

enum ResourceType {
    PHYSICS,
    MODEL,
    SIMPLE
};

struct FResource {
    Referen refSelf;
    ResourceType type;
};

struct FObject {
    Referen refSelf;

    std::string id;
    btVector3 physOffset;

    Referen refModel;
    Referen refPhysics;
};

struct FMain {
    std::string name;
    std::string namesp;

    std::vector<std::string> authors;
    std::string desc;

    std::vector<Referen> refObjects;
};

void compileProject(Path pathToProjectRoot);

}

#endif // PROJECTCOMPILER_H
