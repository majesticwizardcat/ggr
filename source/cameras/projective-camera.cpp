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

Ray ProjectiveCamera::generateRay(const CameraSample& sample) const {
	Point2 centeredFilmPos(
		(2.0f * sample.filmPosition.x - 1.0f) * m_aspectRatio,
		1.0f - 2.0f * sample.filmPosition.y);
	Point2 dxCenteredFilmPos(
		(2.0f * (sample.filmPosition.x + m_dx) - 1.0f) * m_aspectRatio,
		1.0f - 2.0f * sample.filmPosition.y);
	Point2 dyCenteredFilmPos(
		(2.0f * sample.filmPosition.x - 1.0f) * m_aspectRatio,
		1.0f - 2.0f * (sample.filmPosition.y + m_dy));

	Ray main = m_cameraToWorld->applyRay(applyDOF(unproject(centeredFilmPos), sample.lensPosition));
	Ray dxMain = m_cameraToWorld->applyRay(applyDOF(unproject(dxCenteredFilmPos), sample.lensPosition));
	Ray dyMain = m_cameraToWorld->applyRay(applyDOF(unproject(dyCenteredFilmPos), sample.lensPosition));

	main.dxOrigin = dxMain.origin;
	main.dxDirection = dxMain.direction;
	main.dyOrigin = dyMain.origin;
	main.dyDirection = dyMain.direction;
	main.weight = 1.0f;
	main.isCameraRay = true;
	return main;
}

Ray ProjectiveCamera::applyDOF(const Ray& ray, const Point2& lensPosition) const {
	if (m_lensRadius <= 0.0f) {
		return ray;
	}

	Point2 lp = lensPosition * m_lensRadius;
	Point3 fp = ray.point(m_focalDistance / ray.direction.z);
	Point3 origin(lp.x, lp.y, 0.0f);

	Ray r;
	r.origin = origin;
	r.direction = glm::normalize(fp - origin);
	return r;
}

