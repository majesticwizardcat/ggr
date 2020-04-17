#pragma once

class Intersection;

#include "primitives/vector.h"
#include "intersection/surface-point.h"
#include "materials/material.h"
#include "entities/entity.h"

#include <memory>

class Intersection {
public:
	bool hit;
	float t;
	Vector3 wo;
	std::shared_ptr<Material> material;
	SurfacePoint intersectionPoint;
	const LightEntity* light;

	Intersection();
	Intersection(const Intersection& other);
	Intersection(bool hit, float t, const Vector3& wo, const SurfacePoint& intersectionPoint,
		const std::shared_ptr<Material>& material, const LightEntity* light);
	Intersection(bool hit, float t, const Vector3& wo, const SurfacePoint& intersectionPoint);
	Intersection(float t, const Vector3& wo, const SurfacePoint& intersectionPoint);

	void print() const;
	void calculateScreenDifferentials(const Ray& ray);
	bool operator==(const Intersection& other) const;
};

