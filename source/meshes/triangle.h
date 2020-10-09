#pragma once

class Triangle;

#include "primitives/transformation.h"
#include "meshes/vertex.h"
#include "intersection/ray.h"
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
	Vertex interpolate(float w0, float w1, float w2) const;
	bool intersects(const Ray& ray, Intersection* result) const;
	bool intersects(const Ray& ray, float maxT, Intersection* result) const;
	bool intersects(const Ray& ray, float maxT) const;
	SurfacePoint samplePoint(Sampler* sampler) const;

	inline float getArea() const { return m_area; }
	inline bool isDegenerate() const { return m_degenerate; }
};
