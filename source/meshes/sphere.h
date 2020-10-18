#pragma once

#include <cstddef>

class Sphere;

const size_t DEFAULT_CIRCLES = 8;

#include "meshes/triangle-mesh.h"

class Sphere : public TriangleMesh {
public:
	Sphere();
	Sphere(size_t circles);
	Sphere(const Sphere& other) = delete;
};

