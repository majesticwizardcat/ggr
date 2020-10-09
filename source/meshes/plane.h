#pragma once

class Plane;

#include "meshes/triangle-mesh.h"

class Plane : public TriangleMesh {
public:
	Plane();
	Plane(const Plane& other) = delete;
};

