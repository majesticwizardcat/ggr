#include "cameras/orthographic-camera.h"
#include "primitives/transformation.h"

OrthographicCamera::OrthographicCamera(const OrthographicCamera& other) : ProjectiveCamera(other),
	m_scale(other.m_scale) { }
OrthographicCamera::OrthographicCamera(const std::shared_ptr<Transformation>& cameraToWorld,
	int resolutionWidth, int resolutionHeight,
	float lensRadius, float focalDistance, float scale) :
	ProjectiveCamera(cameraToWorld, resolutionWidth, resolutionHeight,
	std::make_shared<Transformation>(transform::orthographicProjection(0.1f, 1000.0f)),
	lensRadius, focalDistance), m_scale(scale) { }

void OrthographicCamera::unproject(Ray* filmPositionRay) const {
	filmPositionRay->origin = m_projection->applyInversePoint(Point4(filmPositionRay->origin * m_scale, 1.0f));
	filmPositionRay->direction = Vector3(0.0f, 0.0f, 1.0f);
	filmPositionRay->dxOrigin = m_projection->applyInversePoint(Point4(filmPositionRay->dxOrigin * m_scale, 1.0f));
	filmPositionRay->dxDirection = Vector3(0.0f, 0.0f, 1.0f);
	filmPositionRay->dyOrigin = m_projection->applyInversePoint(Point4(filmPositionRay->dyOrigin * m_scale, 1.0f));
	filmPositionRay->dyDirection = Vector3(0.0f, 0.0f, 1.0f);
}

std::unique_ptr<Camera> OrthographicCamera::clone() const {
	return std::make_unique<OrthographicCamera>(*this);
}

