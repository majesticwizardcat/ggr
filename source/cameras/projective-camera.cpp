#include "cameras/projective-camera.h"
#include "tools/shading-functions.h"

ProjectiveCamera::ProjectiveCamera(const ProjectiveCamera& other) : Camera(other), 
	m_unprojection(other.m_unprojection), m_lensRadius(other.m_lensRadius),
	m_focalDistance(other.m_focalDistance) { }

ProjectiveCamera::ProjectiveCamera(const Transformation* cameraToWorld,
	int resolutionWidth, int resolutionHeight,
	const Transformation& projection, float lensRadius, float focalDistance) :
	Camera(cameraToWorld, resolutionWidth, resolutionHeight),
	m_lensRadius(lensRadius), m_focalDistance(focalDistance) {
	m_unprojection = projection.inverse() * m_rasterToFilm;
	Point3 worldDeltasX(1.0f, 0.0f, 0.0f);
	Point3 worldDeltasY(0.0f, 1.0f, 0.0f);
	Point3 originZero(0.0f, 0.0f, 0.0f);
	originZero = m_cameraToWorld->applyPoint(m_unprojection.applyPoint(originZero));
	worldDeltasX = m_cameraToWorld->applyPoint(m_unprojection.applyPoint(worldDeltasX));
	worldDeltasY = m_cameraToWorld->applyPoint(m_unprojection.applyPoint(worldDeltasY));
	m_dx = worldDeltasX - originZero;
	m_dy = worldDeltasY - originZero;
}

#include <iostream>
void ProjectiveCamera::applyDOF(Ray* ray, const Point2& lensPosition) const {
	if (m_lensRadius <= 0.0f) {
		return;
	}
	Point2 lp = shading::diskSample(lensPosition);
	float ft = std::abs(m_focalDistance / ray->direction.z);
	Point3 fp = ray->direction * ft;
	ray->origin += Point3(lp * m_lensRadius, 0.0f);
	ray->direction = glm::normalize(fp - ray->origin);
}
