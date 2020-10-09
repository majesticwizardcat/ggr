#include "entities/light-entity.h"

#include <glm/gtx/norm.hpp>

LightEntity::LightEntity(const LightEntity& other) : Entity(other) { }
LightEntity::LightEntity(const Triangle* mesh, const EmissionMaterial* material, int meshID)
	: Entity(mesh, material, meshID) { }

void LightEntity::intersects(const Ray& ray, float maxT, Intersection* result) const {
	if (m_mesh->intersects(ray, maxT, result)) {
		result->hit = true;
		result->material = m_material;
		result->light = this;
		result->intersectionPoint.meshID = m_id;
	}
}

Spectrum LightEntity::emission(const Point3& surfacePoint, const SurfacePoint& lightPoint) const {
	Vector3 direction = glm::normalize(surfacePoint - lightPoint.point);
	return m_material->createBSDF(lightPoint, direction).evaluate(direction, direction);
}

float LightEntity::pdf(const Point3& surfacePoint, const SurfacePoint& lightPoint) const {
	Vector3 direction = surfacePoint - lightPoint.point;
	float distance2 = glm::length2(direction);
	direction = glm::normalize(direction);
	float denom = std::abs(glm::dot(lightPoint.shadingNormal, direction)) * lightPoint.surfaceArea;
	if (denom == 0.0f) {
		return 0.0f;
	}
	return (distance2 + 1.0f) / denom;
}

LightSample LightEntity::sample(Sampler* sampler, const Point3& surfacePoint) const {
	SurfacePoint sampled = m_mesh->samplePoint(sampler);
	sampled.meshID = m_id;
	float lpdf = pdf(surfacePoint, sampled);
	if (lpdf == 0.0f) {
		return LightSample(Spectrum(0.0f), sampled, 0.0f);
	}
	return LightSample(emission(surfacePoint, sampled), sampled, lpdf);
}

