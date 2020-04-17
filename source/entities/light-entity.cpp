#include "entities/light-entity.h"

LightEntity::LightEntity() : Entity() { }
LightEntity::LightEntity(const LightEntity& other) : Entity(other) { }
LightEntity::LightEntity(const std::shared_ptr<Triangle>& mesh,
	const std::shared_ptr<EmissionMaterial>& material, int meshID) : Entity(mesh, material, meshID) { }

void LightEntity::intersects(const Ray& ray, float maxT, Intersection* result) const {
	if (m_mesh->intersects(ray, maxT, result)) {
		result->hit = true;
		result->material = m_material;
		result->light = this;
	}
}

Spectrum LightEntity::emission(const Point3& surfacePoint, const SurfacePoint& lightPoint) const {
	Vector3 direction = (surfacePoint - lightPoint.point).unit();
	return m_material->createBSDF(lightPoint, direction).evaluate(direction, direction);
}

float LightEntity::pdf(const Point3& surfacePoint, const SurfacePoint& lightPoint) const {
	Vector3 direction = surfacePoint - lightPoint.point;
	float distance2 = direction.lengthSquared();
	direction.normalize();
	float denom = std::abs(lightPoint.shadingNormal.dot(direction)) * lightPoint.surfaceArea;
	if (denom == 0.0f) {
		return 0.0f;
	}

	return (distance2 + 1.0f) / denom;
}

LightSample LightEntity::sample(Sampler* sampler, const Point3& surfacePoint) const {
	SurfacePoint sampled = m_mesh->samplePoint(sampler);
	float lpdf = pdf(surfacePoint, sampled);
	if (lpdf == 0.0f) {
		return LightSample(Spectrum(0.0f), sampled, 0.0f);
	}
	return LightSample(emission(surfacePoint, sampled), sampled, lpdf);
}

