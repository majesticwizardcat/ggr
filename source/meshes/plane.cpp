#include "meshes/plane.h"
#include "primitives/point.h"
#include "primitives/vector.h"

Plane::Plane() {
	Point3 p0(-0.5f, -0.5f, 0.0f);
	Point3 p1( 0.5f, -0.5f, 0.0f);
	Point3 p2(-0.5f,  0.5f, 0.0f);
	Point3 p3( 0.5f,  0.5f, 0.0f);

	Point2 uv0(0.0f, 1.0f);
	Point2 uv1(1.0f, 1.0f);
	Point2 uv2(0.0f, 0.0f);
	Point2 uv3(1.0f, 0.0f);

	Vector3 n(0.0f, 0.0f, 1.0f);

	m_vertices.push_back(std::make_shared<Vertex>(p0, uv0, n));
	m_vertices.push_back(std::make_shared<Vertex>(p1, uv1, n));
	m_vertices.push_back(std::make_shared<Vertex>(p2, uv2, n));
	m_vertices.push_back(std::make_shared<Vertex>(p3, uv3, n));

	m_triangles.push_back(std::make_shared<Triangle>(m_vertices[0], m_vertices[1], m_vertices[2]));
	m_triangles.push_back(std::make_shared<Triangle>(m_vertices[1], m_vertices[3], m_vertices[2]));
}

Plane::Plane(const Plane& other) : TriangleMesh(other) { }

