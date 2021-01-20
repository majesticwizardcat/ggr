#include "entities/light-entity.h"

#include <glm/gtx/norm.hpp>

Spectrum LightEntity::emission(const Point3& surfacePoint, const SurfacePoint& lightPoint) const {
	Vector3 direction = surfacePoint - lightPoint.point;
	return m_material->createShader(lightPoint, direction)->evaluate(direction, direction);
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
	ls.pdf = 0.0f;
	Vector3 dir = surfacePoint - ls.sampledPoint.point;
	float distance = glm::length(dir);
	float dist2 = distance * distance;
	float p = 1.0f / (1.0 + dist2);
	if (distance == 0.0f || sampler->getSample() > p) {
		return ls;
	}
	dir /= distance;
	float denom = glm::dot(ls.sampledPoint.shadingNormal, dir) * ls.sampledPoint.surfaceArea;
	if (denom <= 0.0f) {
		return ls;
	}
	ls.pdf = p * dist2 / denom;
	ls.emission = emission(surfacePoint, ls.sampledPoint);
	return ls;
}
