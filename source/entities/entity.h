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
	Entity(const Entity& other) : Entity(other.m_mesh, other.m_material, other.m_id) { }
	Entity(const Triangle* mesh, const Material* material, int id) : m_mesh(mesh),
		m_material(material), m_id(id) { }

	void fillMeshIntersection(float w0, float w1, float w2, Intersection* result) const;
	BoundingBox createBoundingBox() const;
	bool intersects(const Ray& ray, float maxT, EntityIntersection* result) const;

	inline bool intersects(const Ray& ray, float maxT) const {
		return intersects(ray, maxT, nullptr);
	}

	inline const Material* getMaterial() const { return m_material; }
	inline int getID() const { return m_id; }

	virtual inline const LightEntity* getLight() const { return nullptr; }
};

