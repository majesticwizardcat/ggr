#include "entities/light-entity.h"

#include <glm/gtx/norm.hpp>

Spectrum LightEntity::emission(const Point3& surfacePoint, const SurfacePoint& lightPoint) const {
	Vector3 direction = surfacePoint - lightPoint.point;
	return m_material->createBSDF(lightPoint, direction).evaluate(direction, direction);
}

float LightEntity::pdf(const Point3& surfacePoint, const SurfacePoint& lightPoint) const {
	Vector3 direction = surfacePoint - lightPoint.point;
	float distance = glm::length(direction);
	if (distance == 0.0f) {
		return 0.0;
	}

	direction /= distance;
	float denom = glm::dot(lightPoint.shadingNormal, direction) * lightPoint.surfaceArea;
	if (denom <= 0.0f) {
		return 0.0f;
	}
	return distance * distance / denom;
}

LightSample LightEntity::sample(Sampler* sampler, const Point3& surfacePoint) const {
	LightSample ls;
	ls.sampledPoint = m_mesh->samplePoint(sampler);
	ls.sampledPoint.meshID = m_id;
	ls.pdf = pdf(surfacePoint, ls.sampledPoint);
	if (ls.pdf > 0.0f) {
		ls.emission = emission(surfacePoint, ls.sampledPoint);
	}
	return ls;
}

