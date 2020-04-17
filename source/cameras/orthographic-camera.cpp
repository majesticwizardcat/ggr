#include "cameras/orthographic-camera.h"
#include "primitives/transformation.h"

OrthographicCamera::OrthographicCamera() : ProjectiveCamera(), m_scale(0.0f) { }
OrthographicCamera::OrthographicCamera(const OrthographicCamera& other) : ProjectiveCamera(other),
	m_scale(other.m_scale) { }
OrthographicCamera::OrthographicCamera(const std::shared_ptr<Transformation>& cameraToWorld,
	int resolutionWidth, int resolutionHeight,
	float lensRadius, float focalDistance, float scale) :
	ProjectiveCamera(cameraToWorld, resolutionWidth, resolutionHeight,
	std::shared_ptr<Transformation>(
	new Transformation(transform::orthographicProjection(0.1f, 1000.0f))),
	lensRadius, focalDistance), m_scale(scale) { }

Ray OrthographicCamera::unproject(const Point2& filmPosition) const {
	return m_projection->applyInverse(Ray(Point3(filmPosition * m_scale), Vector3(0.0f, 0.0f, 1.0f)));
}

std::unique_ptr<Camera> OrthographicCamera::clone() const {
	return std::unique_ptr<Camera>(new OrthographicCamera(*this));
}

