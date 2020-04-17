#pragma once

class Sphere;

const int DEFAULT_CIRCLES = 8;

#include "meshes/triangle-mesh.h"

class Sphere : public TriangleMesh {
public:
	Sphere();
	Sphere(const Sphere& other);
	Sphere(int circles);
};

