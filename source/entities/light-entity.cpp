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

Spectrum LightEntity::sample(
	Sampler* sampler, const Point3& surfacePoint, 
	SurfacePoint* sampledPoint,
	Vector3* direction, float* pdf, float* lightDist) const 
{
	m_mesh->samplePoint(sampler, sampledPoint);
	sampledPoint->meshID = m_id;
	*pdf = 0.0f;
	*direction = surfacePoint - sampledPoint->point;
	*lightDist = glm::length(*direction);
	float dist2 = *lightDist * *lightDist;
	float p = 1.0f / (1.0 + *lightDist);
	if (dist2 == 0.0f || sampler->getSample() > p) {
		return Spectrum(0.0f);
	}
	*direction /= *lightDist;
	float denom = glm::dot(sampledPoint->shadingNormal, *direction) * sampledPoint->surfaceArea;
	if (denom <= 0.0f) {
		return Spectrum(0.0f);
	}
	*pdf = p * dist2 / denom;
	return emission(surfacePoint, *sampledPoint);
}
