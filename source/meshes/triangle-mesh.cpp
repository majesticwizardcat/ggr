#include "meshes/triangle-mesh.h"

#include <limits>

TriangleMesh::TriangleMesh() { }
TriangleMesh::TriangleMesh(const TriangleMesh& other) {
	for (const std::shared_ptr<Triangle>& t : other.m_triangles) {
		addTriangle(*t);
	}
}

TriangleMesh::TriangleMesh(const std::vector<std::shared_ptr<Vertex>>& vertices, const std::vector<int>& indices) :
	m_vertices(vertices) {
	for (int i = 0; i < indices.size(); i += 3) {
		m_triangles.push_back(std::shared_ptr<Triangle>(
			new Triangle(vertices[indices[i]],
			vertices[indices[i + 1]],
			vertices[indices[i + 2]])));
	}
}

void TriangleMesh::addTriangle(const Triangle& t) {
	std::shared_ptr<Vertex> v0(new Vertex(*t.v0));
	std::shared_ptr<Vertex> v1(new Vertex(*t.v0));
	std::shared_ptr<Vertex> v2(new Vertex(*t.v0));
	m_vertices.push_back(v0);
	m_vertices.push_back(v1);
	m_vertices.push_back(v2);
	m_triangles.push_back(std::shared_ptr<Triangle>(new Triangle(v0, v1, v2)));
}

void TriangleMesh::transform(const Transformation& transformation) {
	for (const std::shared_ptr<Vertex>& v : m_vertices) {
		v->position = transformation.apply(v->position);
		v->normal = transformation.apply(v->normal);
	}

	for (int i = 0; i < m_triangles.size(); ++i) {
		m_triangles[i]->calculateGeometry();
	}
}

std::vector<std::shared_ptr<Triangle>> TriangleMesh::getTriangles() const {
	return m_triangles;
}

