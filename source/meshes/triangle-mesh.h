#pragma once

class TriangleMesh;

#include "meshes/vertex.h"
#include "meshes/triangle.h"
#include "intersection/ray.h"
#include "intersection/intersection.h"
#include "primitives/transformation.h"
#include "samplers/sampler.h"

#include <memory>
#include <vector>

class TriangleMesh {
protected:
	std::vector<Vertex> m_vertices;
	std::vector<std::unique_ptr<Triangle>> m_triangles;

	TriangleMesh() { }

public:
	TriangleMesh(const TriangleMesh& other) = delete;
	TriangleMesh(const std::vector<Vertex>& vertices, const std::vector<int>& indices);
	virtual ~TriangleMesh() { }

	void transform(const Transformation& transformation);
	inline const std::unique_ptr<Triangle>* getTriangles() const { return m_triangles.data(); }
	inline size_t getNumberOfTriangles() const { return m_triangles.size(); }
};
