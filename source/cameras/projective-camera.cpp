#include "cameras/projective-camera.h"

ProjectiveCamera::ProjectiveCamera(const ProjectiveCamera& other) : Camera(other), 
	m_projection(other.m_projection), m_lensRadius(other.m_lensRadius),
	m_focalDistance(other.m_focalDistance) { }
ProjectiveCamera::ProjectiveCamera(const Transformation* cameraToWorld,
	int resolutionWidth, int resolutionHeight,
	const Transformation& projection, float lensRadius, float focalDistance) :
	Camera(cameraToWorld, resolutionWidth, resolutionHeight),
	m_lensRadius(lensRadius), m_focalDistance(focalDistance) {
	Transformation prjScaleTranslate = transform::translate(-1.0f, 1.0f, 0.0f)
		* transform::scale(2.0f, -2.0f, 1.0f);
	m_projection = prjScaleTranslate.inverse() * projection;
	Point3 worldDeltasX(1.0f / (float)(resolutionWidth), 0.0f, 0.0f);
	Point3 worldDeltasY(0.0f, 1.0f / (float)(resolutionHeight), 0.0f);
	Point3 originZero(0.0f, 0.0f, 0.0f);
	originZero = m_cameraToWorld->applyPoint(m_projection.applyInversePoint(originZero));
	worldDeltasX = m_cameraToWorld->applyPoint(m_projection.applyInversePoint(worldDeltasX));
	worldDeltasY = m_cameraToWorld->applyPoint(m_projection.applyInversePoint(worldDeltasY));
	originZero = m_projection.applyInversePoint(originZero);
	m_dx = worldDeltasX - originZero;
	m_dy = worldDeltasY - originZero;
}

void ProjectiveCamera::applyDOF(Ray* ray, const Point2& lensPosition) const {
	if (m_lensRadius <= 0.0f) {
		return;
	}
	Point3 fp = ray->point(m_focalDistance / ray->direction.z);
	ray->origin += Point3(lensPosition * m_lensRadius, 0.0f);
	ray->direction = glm::normalize(fp - ray->origin);
}
