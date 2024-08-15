#pragma once

struct EntityIntersection;

#include "entities/entity.h"
#include "primitives/point.h"

#include <limits>

struct EntityIntersection {
	float t;
	const Entity* entity;
	struct {
		float w0;
		float w1;
		float w2;
	} triangleWeights;
	
	EntityIntersection() : t(std::numeric_limits<float>::max()), entity(nullptr) { }

	EntityIntersection(const EntityIntersection& other) : 
		t(other.t), entity(other.entity), triangleWeights(other.triangleWeights) { }
};
