#include "meshes/triangle-mesh.h"

TriangleMesh::TriangleMesh(const std::vector<Vertex>& vertices, const std::vector<int>& indices) 
	: m_vertices(vertices) {
	for (size_t i = 0; i < indices.size(); i += 3) {
		m_triangles.push_back(std::make_unique<Triangle>(
			&m_vertices[indices[i]],
			&m_vertices[indices[i + 1]],
			&m_vertices[indices[i + 2]]));
		m_triangles.back()->calculateGeometry();
	}
}

void TriangleMesh::transform(const Transformation& transformation) {
	for (Vertex& v : m_vertices) {
		v.position = transformation.applyPoint(v.position);
		v.normal = transformation.applyNormal(v.normal);
	}

	for (std::unique_ptr<Triangle>& t : m_triangles) {
		t->calculateGeometry();
	}
}
