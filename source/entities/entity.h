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
	int m_id;
	const Triangle* m_mesh;
	const Material* m_material;

public:
	Entity() = delete;
	Entity(const Entity& other);
	Entity(const Triangle* mesh, const Material* material, int meshID);

	virtual void intersects(const Ray& ray, float maxT, Intersection* result) const;
	bool intersects(const Ray& ray, float maxT) const;
	int getID() const;
	BoundingBox createBoundingBox() const;
};

