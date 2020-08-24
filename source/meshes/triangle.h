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
	int id;
	std::shared_ptr<Vertex> v0;
	std::shared_ptr<Vertex> v1;
	std::shared_ptr<Vertex> v2;

	Triangle();
	Triangle(const std::shared_ptr<Vertex>& v0, const std::shared_ptr<Vertex>& v1,
		const std::shared_ptr<Vertex>& v2);
	Triangle(const Triangle& other);

	void calculateGeometry();
	Vertex interpolate(float w0, float w1, float w2) const;
	bool intersects(const Ray& ray, Intersection* result) const;
	bool intersects(const Ray& ray, float maxT, Intersection* result) const;
	bool intersects(const Ray& ray, float maxT) const;
	SurfacePoint samplePoint(Sampler* sampler) const;
	float getArea() const;
};

