#pragma once

class Entity;
class LightEntity;

#include "meshes/triangle-mesh.h"
#include "materials/material.h"
#include "intersection/intersection.h"
#include "accelerators/bounding-box.h"

#include <memory>

class Entity {
protected:
	std::shared_ptr<Triangle> m_mesh;
	std::shared_ptr<Material> m_material;

public:
	Entity();
	Entity(const Entity& other);
	Entity(const std::shared_ptr<Triangle>& mesh, const std::shared_ptr<Material>& material, int meshID);

	virtual void intersects(const Ray& ray, float maxT, Intersection* result) const;
	bool intersects(const Ray& ray, float maxT) const;
	int getMeshID() const;

	BoundingBox createBoundingBox() const;
};

