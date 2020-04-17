#pragma once

class Cube;

#include "meshes/triangle-mesh.h"

class Cube : public TriangleMesh {
public:
	Cube();
	Cube(const Cube& other);
};

