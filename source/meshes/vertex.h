#pragma once

struct Vertex;

#include "primitives/vector.h"
#include "primitives/point.h"

struct Vertex {
	Point3 position;
	Point2 uv;
	Vector3 normal;

	Vertex() { }
	Vertex(const Vertex& other) : 
		position(other.position), uv(other.uv), normal(other.normal) { }

	Vertex(const Point3& position, const Point2& uv, const Vector3& normal) : 
		position(position), uv(uv), normal(normal) { }
};

