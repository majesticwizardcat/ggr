#include "cameras/projective-camera.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>
ProjectiveCamera::ProjectiveCamera(const ProjectiveCamera& other) : Camera(other), 
	m_projection(other.m_projection), m_lensRadius(other.m_lensRadius),
	m_focalDistance(other.m_focalDistance) { }
ProjectiveCamera::ProjectiveCamera(const std::shared_ptr<Transformation>& cameraToWorld,
	int resolutionWidth, int resolutionHeight,
	const std::shared_ptr<Transformation>& projection, float lensRadius, float focalDistance) :
	Camera(cameraToWorld, resolutionWidth, resolutionHeight),
	m_projection(projection), m_lensRadius(lensRadius), m_focalDistance(focalDistance) { }

void ProjectiveCamera::generateRay(Ray* ray, const CameraSample& sample) const {
	ray->origin.x = 2.0f * sample.filmPosition.x - 1.0f;
	ray->origin.y = 1.0f - 2.0f * sample.filmPosition.y;
	ray->origin.z = 0.0f;
	ray->dxOrigin.x = ray->origin.x + 2.0f * m_dx;
	ray->dxOrigin.y = ray->origin.y;
	ray->dxOrigin.z = 0.0f;
	ray->dyOrigin.x = ray->origin.x;
	ray->dyOrigin.y = ray->origin.y + 2.0f * m_dy;
	ray->dyOrigin.z = 0.0f;

	unproject(ray);
	applyDOF(ray, sample.lensPosition);
	m_cameraToWorld->applyRay(ray);
	ray->weight = 1.0f;
	ray->isCameraRay = true;
}

void ProjectiveCamera::applyDOF(Ray* ray, const Point2& lensPosition) const {
	if (m_lensRadius <= 0.0f) {
		return;
	}

	Point3 lp = Point3(lensPosition * m_lensRadius, 0.0f);
	Point3 fp = ray->point(m_focalDistance / ray->direction.z);
	ray->origin += lp;
	ray->direction = glm::normalize(fp - ray->origin);
	ray->dxOrigin += lp;
	ray->dxDirection = glm::normalize(fp - ray->dxOrigin);
	ray->dyOrigin += lp;
	ray->dyDirection = glm::normalize(fp - ray->dyOrigin);
}

