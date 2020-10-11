#include "cameras/perspective-camera.h"
#include "primitives/transformation.h"

PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera& other) :
	ProjectiveCamera(other) { }
PerspectiveCamera::PerspectiveCamera(const std::shared_ptr<Transformation>& cameraToWorld,
	int resolutionWidth, int resolutionHeight,
	float lensRadius, float focalDistance, float fov) :
	ProjectiveCamera(cameraToWorld, resolutionWidth, resolutionHeight,
	std::make_shared<Transformation>(transform::perspectiveProjection(fov, 0.01f, 1000.0f)),
	lensRadius, focalDistance) { }

void PerspectiveCamera::unproject(Ray* filmPositionRay) const {
	filmPositionRay->direction = glm::normalize(m_projection->applyInversePoint(filmPositionRay->origin));
	filmPositionRay->dxDirection = glm::normalize(m_projection->applyInversePoint(filmPositionRay->dxOrigin));
	filmPositionRay->dyDirection = glm::normalize(m_projection->applyInversePoint(filmPositionRay->dyOrigin));

	filmPositionRay->origin = Point3(0.0f);
	filmPositionRay->dxOrigin = Point3(0.0f);
	filmPositionRay->dyOrigin = Point3(0.0f);
}

std::unique_ptr<Camera> PerspectiveCamera::clone() const {
	return std::make_unique<PerspectiveCamera>(*this);
}

