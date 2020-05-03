#pragma once

class Accelerator;

#include "intersection/intersection.h"
#include "intersection/ray.h"
#include "entities/entity.h"

#include <memory>

class Accelerator {
public:
	virtual void addMesh(const std::vector<std::shared_ptr<Entity>>& mesh) = 0;
	virtual void initialize() = 0;
	virtual void intersects(const Ray& ray, Intersection* result) const = 0;
	virtual void intersects(const Ray& ray, const SurfacePoint& surface, Intersection* result) const = 0;
	virtual bool intersects(const Ray& ray, const SurfacePoint& surface, float maxT) const = 0;
	virtual std::unique_ptr<Accelerator> clone() const = 0;
};

