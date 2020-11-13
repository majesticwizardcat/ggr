#include "cameras/orthographic-camera.h"
#include "primitives/transformation.h"

OrthographicCamera::OrthographicCamera(const OrthographicCamera& other) : ProjectiveCamera(other) { }
OrthographicCamera::OrthographicCamera(const std::shared_ptr<Transformation>& cameraToWorld,
	int resolutionWidth, int resolutionHeight,
	float lensRadius, float focalDistance, float scale) :
	ProjectiveCamera(cameraToWorld, resolutionWidth, resolutionHeight,
	std::make_shared<Transformation>(transform::orthographicProjection(0.1f, scale, scale, 1000.0f)),
	lensRadius, focalDistance) { }

void OrthographicCamera::unproject(Ray* filmPositionRay) const {
	filmPositionRay->origin = m_projection->applyInversePoint(Point4(filmPositionRay->origin, 1.0f));
	filmPositionRay->direction = Vector3(0.0f, 0.0f, 1.0f);
	filmPositionRay->dxOrigin = m_projection->applyInversePoint(Point4(filmPositionRay->dxOrigin, 1.0f));
	filmPositionRay->dxDirection = Vector3(0.0f, 0.0f, 1.0f);
	filmPositionRay->dyOrigin = m_projection->applyInversePoint(Point4(filmPositionRay->dyOrigin, 1.0f));
	filmPositionRay->dyDirection = Vector3(0.0f, 0.0f, 1.0f);
}

std::unique_ptr<Camera> OrthographicCamera::clone() const {
	return std::make_unique<OrthographicCamera>(*this);
}

