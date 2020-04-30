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

Ray PerspectiveCamera::unproject(const Point2& filmPosition) const {
	Point3 unprojected = m_projection->applyInverse(Point3(filmPosition));
	Vector3 fp(unprojected);

	Ray r;
	r.origin = Point3(0.0f);
	r.direction = fp.unit();
	return r;
}

std::unique_ptr<Camera> PerspectiveCamera::clone() const {
	return std::make_unique<PerspectiveCamera>(*this);
}

