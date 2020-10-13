#pragma once

class Triangle;

#include "primitives/transformation.h"
#include "meshes/vertex.h"
#include "intersection/ray.h"
#include "intersection/entity-intersection.h"
#include "intersection/intersection.h"
#include "samplers/sampler.h"

#include <memory>

class Triangle {
private:
	Vector3 m_dpdu;
	Vector3 m_dpdv;
	Vector3 m_geometricNormal;
	Vector3 m_tangent;
	float m_area;
	bool m_degenerate;

public:
	const Vertex* v0;
	const Vertex* v1;
	const Vertex* v2;

	Triangle() = delete;
	Triangle(const Triangle& other) = delete;
	Triangle(const Vertex* v0, const Vertex* v1, const Vertex* v2);

	void calculateGeometry();
	bool intersects(const Ray& ray, float maxT, EntityIntersection* result) const;
	void fillIntersection(float w0, float w1, float w2, Intersection* result) const;
	SurfacePoint samplePoint(Sampler* sampler) const;

	inline float getArea() const { return m_area; }
	inline bool isDegenerate() const { return m_degenerate; }
};
