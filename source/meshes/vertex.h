#pragma once

class Vertex;

#include "primitives/vector.h"
#include "primitives/point.h"

class Vertex {
public:
	Point3 position;
	Point2 uv;
	Vector3 normal;

	Vertex();
	Vertex(const Vertex& other);
	Vertex(const Point3& position, const Point2& uv, const Vector3& normal);
};

