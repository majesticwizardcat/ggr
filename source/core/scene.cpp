#include "core/scene.h"
#include "textures/color-texture.h"
#include "accelerators/bb-accelerator.h"
#include "tools/constants.h"

#include <limits>

Scene::Scene() : Scene(
	std::make_shared<Skybox>(std::make_shared<ColorTexture>(Spectrum(0.0f)))) {
	m_nextID = 0;
}
Scene::Scene(const Scene& other) : m_skybox(other.m_skybox), m_accelerator(other.m_accelerator->clone()),
	m_lights(other.m_lights), m_nextID(other.m_nextID) { }
Scene::Scene(const std::shared_ptr<Skybox>& skybox) : m_skybox(skybox) {
	m_accelerator = std::make_unique<BBAccelerator>();
	m_nextID = 0;
}

void Scene::addEntity(const std::shared_ptr<TriangleMesh>& mesh, const std::shared_ptr<Material>& material) {
	std::vector<std::shared_ptr<Triangle>> triangles = mesh->getTriangles();
	std::vector<std::shared_ptr<Entity>> entities;
	for (const std::shared_ptr<Triangle>& tri : triangles) {
		entities.push_back(std::make_shared<Entity>(tri, material, m_nextID));
		m_nextID++;
	}
	m_accelerator->addMesh(entities);
}

void Scene::addLight(const std::shared_ptr<TriangleMesh>& mesh, const std::shared_ptr<EmissionMaterial>& emissionMat) {
	std::vector<std::shared_ptr<Triangle>> triangles = mesh->getTriangles();
	std::vector<std::shared_ptr<Entity>> entities;
	for (const std::shared_ptr<Triangle>& tri : triangles) {
		std::shared_ptr<LightEntity> l = std::make_shared<LightEntity>(tri, emissionMat, m_nextID);
		entities.push_back(l);
		m_lights.push_back(l);
		m_nextID++;
	}
	m_accelerator->addMesh(entities);
}

void Scene::initializeAccelerator() {
	m_accelerator->initialize();
}

Intersection Scene::intersects(const Ray& ray) const {
	return intersects(ray, std::numeric_limits<float>::max());
}

Intersection Scene::intersects(const Ray& ray, float maxT) const {
	Ray r(ray);
	r.createRaySpace();

	Intersection result;
	result.light = nullptr;
	result.t = maxT;
	result.wo = -ray.direction;

	m_accelerator->intersects(r, &result);
	result.calculateScreenDifferentials(ray);
	return result;
}

Intersection Scene::intersects(const SurfacePoint& surface, const Vector3& direction) const {
	return intersects(surface, direction, std::numeric_limits<float>::max());
}

Intersection Scene::intersects(const SurfacePoint& surface, const Vector3& direction, float maxT) const {
	Ray r(surface.point, direction);
	r.createRaySpace();

	Intersection result;
	result.light = nullptr;
	result.t = maxT;
	result.wo = -direction;

	m_accelerator->intersects(r, surface, &result);
	return result;
}

bool Scene::areUnoccluded(const SurfacePoint& p0, const SurfacePoint& p1) const {
	Vector3 p0p1 = p1.point - p0.point;
	float l2 = p0p1.lengthSquared();
	if (l2 <= 0.01f) {
		return true;
	}

	Intersection i = intersects(p0, p0p1, l2 + 2.0f * ERROR);
	return i.hit && i.intersectionPoint.meshID == p1.meshID;
}

bool Scene::isIntersected(const SurfacePoint& surface, const Vector3& direction) const {
	Ray r(surface.point, direction);
	r.createRaySpace();
	return m_accelerator->intersects(r, surface, std::numeric_limits<float>::max());
}

int Scene::getNumberOfLights() const {
	return m_lights.size();
}

const LightEntity* Scene::getLight(int index) const {
	return m_lights[index].get();
}

const Skybox* Scene::getSkybox() const {
	return m_skybox.get();
}

