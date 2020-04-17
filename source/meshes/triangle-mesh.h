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
	std::vector<std::shared_ptr<Vertex>> m_vertices;
	std::vector<std::shared_ptr<Triangle>> m_triangles;

public:
	TriangleMesh();
	TriangleMesh(const TriangleMesh& other);
	TriangleMesh(const std::vector<std::shared_ptr<Vertex>>& vertices, const std::vector<int>& indices);

	void addTriangle(const Triangle& t);
	void transform(const Transformation& transformation);
	std::vector<std::shared_ptr<Triangle>> getTriangles() const;
};

