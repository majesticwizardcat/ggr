#include "cameras/orthographic-camera.h"
#include "primitives/transformation.h"

OrthographicCamera::OrthographicCamera(const OrthographicCamera& other) : ProjectiveCamera(other) { }

OrthographicCamera::OrthographicCamera(const Transformation* cameraToWorld,
	int resolutionWidth, int resolutionHeight,
	float lensRadius, float focalDistance, float scale) :
	ProjectiveCamera(cameraToWorld, resolutionWidth, resolutionHeight,
	transform::orthographicProjection(-scale / 2.0f, -scale / 2.0f, scale / 2.0f, scale / 2.0f),
	lensRadius, focalDistance) { }

void OrthographicCamera::generateRay(Ray* ray, const CameraSample& sample) const {
	ray->origin = Point3(m_unprojection.applyPoint(Point3(sample.filmPosition, 0.0f)));
	ray->direction = Point3(0.0f, 0.0f, -1.0f);
	applyDOF(ray, sample.lensPosition);
	ray->direction = m_cameraToWorld->applyVector(ray->direction);
	ray->origin = m_cameraToWorld->applyPoint(ray->origin);
	ray->dxDirection = ray->direction;
	ray->dyDirection = ray->direction;
	ray->dxOrigin = ray->origin + m_dx;
	ray->dyOrigin = ray->origin + m_dy;
	ray->weight = 1.0f;
	ray->isCameraRay = true;
}

std::unique_ptr<Camera> OrthographicCamera::clone() const {
	return std::make_unique<OrthographicCamera>(*this);
}

