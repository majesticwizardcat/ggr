#include "meshes/vertex.h"

Vertex::Vertex() { }
Vertex::Vertex(const Vertex& other) : Vertex(other.position, other.uv, other.normal) { }
Vertex::Vertex(const Point3& position, const Point2& uv, const Normal& normal) : position(position),
	uv(uv), normal(normal) { }

