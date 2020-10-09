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
	std::vector<std::unique_ptr<Entity>> m_entities;
	std::vector<LightEntity*> m_lights;
	Skybox* m_skybox;
	int m_nextID;

public:
	Scene() = delete;
	Scene(const Scene& other) = delete;
	Scene(Skybox* skybox);

	void addEntity(const TriangleMesh* mesh, const Material* material);
	void addLight(const TriangleMesh* mesh, const EmissionMaterial* emissionMat);
	void initializeAccelerator();

	Intersection intersects(const Ray& ray) const;
	Intersection intersects(const Ray& ray, float maxT) const;
	Intersection intersects(const SurfacePoint& surface, const Vector3& direction) const;
	Intersection intersects(const SurfacePoint& surface, const Vector3& direction, float maxT) const;

	bool areUnoccluded(const SurfacePoint& p0, const SurfacePoint& p1) const;
	bool isIntersected(const SurfacePoint& surface, const Vector3& direction) const;

	inline size_t getNumberOfLights() const { return m_lights.size(); }
	inline const LightEntity* getLight(int index) const { return m_lights[index]; }
	inline const Skybox* getSkybox() const { return m_skybox; }
};

