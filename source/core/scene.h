#pragma once

class Scene;

#include "entities/entity.h"
#include "entities/light-entity.h"
#include "materials/material.h"
#include "lighting/skybox.h"
#include "meshes/triangle-mesh.h"
#include "accelerators/bb-accelerator.h"

#include <memory>
#include <vector>

class Scene {
private:
	BBAccelerator m_accelerator;
	std::vector<int> m_objectIndices;
	std::vector<std::unique_ptr<Entity>> m_entities;
	std::vector<LightEntity*> m_lights;
	Skybox* m_skybox;
	int m_nextID;

	void fillIntersectionFromEntity(const EntityIntersection& entityIntersection,
		Intersection* result) const;

public:
	Scene() = delete;
	Scene(const Scene& other) = delete;
	Scene(Skybox* skybox);

	void addEntity(const TriangleMesh* mesh, const Material* material);
	void addLight(const TriangleMesh* mesh, const EmissionMaterial* emissionMat);
	void initializeAccelerator();
	void intersects(Ray* ray, Intersection* result) const;
	void intersects(const SurfacePoint& surface, const Vector3& direction, Intersection* result) const;

	inline bool areUnoccluded(const SurfacePoint& p0, const Point3& p,
		const Vector3& dir, float distance) const {
		Ray ray(p0.point, dir);
		ray.createRaySpace();
		return !m_accelerator.intersectsAny(ray, p0, distance - TWO_ERROR);
	}

	inline bool areUnoccluded(const SurfacePoint& p0, const SurfacePoint& p1) const {
		Vector3 p0p1 = p1.point - p0.point;
		float l = glm::length(p0p1);
		return areUnoccluded(p0, p1.point, p0p1 / l, l);
	}

	inline const int getNumberOfLights() const { return m_lights.size(); }
	inline const LightEntity* getLight(int index) const { return m_lights[index]; }
	inline const Skybox* getSkybox() const { return m_skybox; }
};

