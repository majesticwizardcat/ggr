#include "core/scene.h"
#include "textures/color-texture.h"
#include "tools/constants.h"

#include <limits>
#include <glm/gtx/norm.hpp>

Scene::Scene(Skybox* skybox) : m_skybox(skybox), m_nextID(0) { }

void Scene::addEntity(const TriangleMesh* mesh, const Material* material) {
	const std::unique_ptr<Triangle>* triangles = mesh->getTriangles();
	for (size_t i = 0; i < mesh->getNumberOfTriangles(); ++i) {
		if (!triangles[i]->isDegenerate()) {
			m_entities.push_back(std::make_unique<Entity>(triangles[i].get(), material, m_nextID));
			m_nextID++;
		}
	}
}

void Scene::addLight(const TriangleMesh* mesh, const EmissionMaterial* emissionMat) {
	const std::unique_ptr<Triangle>* triangles = mesh->getTriangles();
	for (size_t i = 0; i < mesh->getNumberOfTriangles(); ++i) {
		if (!triangles[i]->isDegenerate()) {
			auto le = std::make_unique<LightEntity>(triangles[i].get(), emissionMat, m_nextID);
			m_lights.push_back(le.get());
			m_entities.push_back(std::move(le));
			m_nextID++;
		}
	}
}

void Scene::initializeAccelerator() {
	m_accelerator.initialize(m_entities.data(), m_entities.size());
}

Intersection Scene::intersects(const Ray& ray) const {
	return intersects(ray, std::numeric_limits<float>::max());
}

Intersection Scene::intersects(const Ray& ray, float maxT) const {
	Ray r(ray);
	r.createRaySpace();

	Intersection result;
	result.t = maxT;
	result.wo = -ray.direction;

	m_accelerator.intersects(r, &result);
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
	result.t = maxT;
	result.wo = -direction;

	m_accelerator.intersects(r, surface, &result);
	return result;
}

bool Scene::areUnoccluded(const SurfacePoint& p0, const SurfacePoint& p1) const {
	Vector3 p0p1 = p1.point - p0.point;
	Intersection i = intersects(p0, glm::normalize(p0p1), std::numeric_limits<float>::max());
	return i.hit && i.intersectionPoint.meshID == p1.meshID;
}

bool Scene::isIntersected(const SurfacePoint& surface, const Vector3& direction) const {
	Ray r(surface.point, direction);
	r.createRaySpace();
	return m_accelerator.intersects(r, surface, std::numeric_limits<float>::max());
}
