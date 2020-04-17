#include "core/scene.h"
#include "textures/color-texture.h"

#include <limits>

Scene::Scene() : Scene(
	std::shared_ptr<Skybox>(new Skybox(std::shared_ptr<Texture>(new ColorTexture(Spectrum(0.0f)))))) { }
Scene::Scene(const Scene& other) : m_skybox(other.m_skybox), m_entities(other.m_entities),
	m_lights(other.m_lights) { }
Scene::Scene(const std::shared_ptr<Skybox>& skybox) : m_skybox(skybox) { }

void Scene::addEntity(const std::shared_ptr<TriangleMesh>& mesh, const std::shared_ptr<Material>& material) {
	std::vector<std::shared_ptr<Triangle>> triangles = mesh->getTriangles();
	for (const std::shared_ptr<Triangle>& tri : triangles) {
		m_entities.push_back(std::shared_ptr<Entity>(new Entity(tri, material, m_entities.size())));
	}
}

void Scene::addLight(const std::shared_ptr<TriangleMesh>& mesh, const std::shared_ptr<EmissionMaterial>& emissionMat) {
	std::vector<std::shared_ptr<Triangle>> triangles = mesh->getTriangles();
	for (const std::shared_ptr<Triangle>& tri : triangles) {
		std::shared_ptr<LightEntity> l(new LightEntity(tri, emissionMat, m_entities.size()));
		m_entities.push_back(l);
		m_lights.push_back(l);
	}
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

	for (const std::shared_ptr<Entity>& entity : m_entities) {
		entity->intersects(r, result.t, &result);
	}

	result.calculateScreenDifferentials(ray);
	return result;
}

Intersection Scene::intersects(const SurfacePoint& surface, const Vector3& direction) const {
	Ray r(surface.point, direction);
	r.createRaySpace();

	Intersection result;
	result.light = nullptr;
	result.t = std::numeric_limits<float>::max();
	result.wo = -direction;

	for (const std::shared_ptr<Entity>& entity : m_entities) {
		if (entity->getMeshID() != surface.meshID) {
			entity->intersects(r, result.t, &result);
		}
	}

	return result;
}

bool Scene::areUnoccluded(const SurfacePoint& p0, const SurfacePoint& p1) const {
	Vector3 p0p1 = p1.point - p0.point;
	if (p0p1.lengthSquared() <= 0.1f) {
		return true;
	}

	Intersection i = intersects(p0, p0p1);
	return i.hit && i.intersectionPoint.meshID == p1.meshID;
}

bool Scene::isIntersected(const SurfacePoint& surface, const Vector3& direction) const {
	Ray r(surface.point, direction);
	r.createRaySpace();

	for (const std::shared_ptr<Entity>& entity : m_entities) {
		if (entity->getMeshID() != surface.meshID
			&& entity->intersects(r, std::numeric_limits<float>::max())) {
			return true;
		}
	}

	return false;
}

int Scene::getNumberOfLights() const {
	return m_lights.size();
}

LightEntity* Scene::getLight(int index) const {
	return m_lights[index].get();
}

Skybox* Scene::getSkybox() const {
	return m_skybox.get();
}

