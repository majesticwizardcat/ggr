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

void Scene::fillIntersectionFromEntity(const EntityIntersection& entityIntersection,
	Intersection* result) const {
		entityIntersection.entity->fillMeshIntersection(entityIntersection.triangleWeights.w0,
			entityIntersection.triangleWeights.w1, entityIntersection.triangleWeights.w2, result);
		result->hit = true;
		result->light = entityIntersection.entity->getLight();
		result->intersectionPoint.meshID = entityIntersection.entity->getID();
		result->material = entityIntersection.entity->getMaterial();
}

void Scene::intersects(Ray* ray, Intersection* result, float maxT) const {
	EntityIntersection entityIntersection;
	entityIntersection.t = maxT;
	ray->createRaySpace();
	if (m_accelerator.intersects(*ray, &entityIntersection)) {
		fillIntersectionFromEntity(entityIntersection, result);
		result->wo = -ray->direction;
		return;
	}
	result->light = nullptr;
	result->hit = false;
}

void Scene::intersects(const SurfacePoint& surface, const Vector3& direction,
	Intersection* result, float maxT) const {
	Ray ray(surface.point, direction);
	EntityIntersection entityIntersection;
	entityIntersection.t = maxT;
	ray.createRaySpace();
	if (m_accelerator.intersects(ray, surface, &entityIntersection)) {
		fillIntersectionFromEntity(entityIntersection, result);
		result->wo = -ray.direction;
		return;
	}
	result->light = nullptr;
	result->hit = false;
}

bool Scene::areUnoccluded(const SurfacePoint& p0, const SurfacePoint& p1) const {
	Vector3 p0p1 = p1.point - p0.point;
	float l = glm::length(p0p1);
	Ray ray(p0.point, p0p1 * (1.0f / l));
	ray.createRaySpace();
	return !m_accelerator.intersectsAny(ray, p0, l - ERROR);
}
