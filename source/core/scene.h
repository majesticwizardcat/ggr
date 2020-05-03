#pragma once

class Scene;

#include "entities/entity.h"
#include "entities/light-entity.h"
#include "materials/material.h"
#include "lighting/skybox.h"
#include "meshes/triangle-mesh.h"
#include "accelerators/accelerator.h"

#include <memory>
#include <vector>

class Scene {
private:
	std::unique_ptr<Accelerator> m_accelerator;
	std::vector<std::shared_ptr<LightEntity>> m_lights;
	std::shared_ptr<Skybox> m_skybox;
	int m_nextID;

public:
	Scene();
	Scene(const Scene& other);
	Scene(const std::shared_ptr<Skybox>& skybox);

	void addEntity(const std::shared_ptr<TriangleMesh>& mesh, const std::shared_ptr<Material>& material);
	void addLight(const std::shared_ptr<TriangleMesh>& mesh, const std::shared_ptr<EmissionMaterial>& emissionMat);
	void initializeAccelerator();

	Intersection intersects(const Ray& ray) const;
	Intersection intersects(const Ray& ray, float maxT) const;
	Intersection intersects(const SurfacePoint& surface, const Vector3& direction) const;
	Intersection intersects(const SurfacePoint& surface, const Vector3& direction, float maxT) const;

	bool areUnoccluded(const SurfacePoint& p0, const SurfacePoint& p1) const;
	bool isIntersected(const SurfacePoint& surface, const Vector3& direction) const;

	int getNumberOfLights() const;
	const LightEntity* getLight(int index) const;
	const Skybox* getSkybox() const;
};

