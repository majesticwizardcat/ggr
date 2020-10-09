#pragma once

class Intersection;

#include "primitives/vector.h"
#include "intersection/surface-point.h"
#include "materials/material.h"
#include "entities/entity.h"

class Intersection {
public:
	bool hit;
	float t;
	Vector3 wo;
	SurfacePoint intersectionPoint;
	const Material* material;
	const LightEntity* light;

	Intersection() : hit(false), t(std::numeric_limits<float>::max()), wo(0.0f),
		material(nullptr), light(nullptr) { }

	void calculateScreenDifferentials(const Ray& ray);
};

