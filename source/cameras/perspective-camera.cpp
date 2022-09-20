#include "cameras/perspective-camera.h"
#include "primitives/transformation.h"

PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera& other) :
	ProjectiveCamera(other) { }

PerspectiveCamera::PerspectiveCamera(const Transformation* cameraToWorld,
	int resolutionWidth, int resolutionHeight,
	float lensRadius, float focalDistance, float fov) :
	ProjectiveCamera(cameraToWorld, resolutionWidth, resolutionHeight,
	transform::perspectiveProjection(fov, (float) resolutionWidth / (float) resolutionHeight, 0.01f, 1000.0f),
	lensRadius, focalDistance) { }

void PerspectiveCamera::generateRay(Ray* ray, const CameraSample& sample) const {
	ray->direction = glm::normalize(m_unprojection.applyPoint(Point3(sample.filmPosition, 0.0f)));
	ray->origin = Point3(0.0f);
	applyDOF(ray, sample.lensPosition);
	ray->direction = glm::normalize(m_cameraToWorld->applyVector(ray->direction));
	ray->origin = m_cameraToWorld->applyPoint(ray->origin);
	ray->dxDirection = ray->direction + m_dx;
	ray->dyDirection = ray->direction + m_dy;
	ray->dxOrigin = ray->origin;
	ray->dyOrigin = ray->origin;
	ray->weight = 1.0f;
	ray->isCameraRay = true;
}

std::unique_ptr<Camera> PerspectiveCamera::clone() const {
	return std::make_unique<PerspectiveCamera>(*this);
}
