#pragma once

class PerspectiveCamera;

#include "cameras/projective-camera.h"

class PerspectiveCamera : public ProjectiveCamera {
public:
	PerspectiveCamera() = delete;
	PerspectiveCamera(const PerspectiveCamera& other);
	PerspectiveCamera(const std::shared_ptr<Transformation>& cameraToWorld,
		int resolutionWidth, int resolutionHeight,
		float lensRadius, float focalDistance, float fov);

	void unproject(Ray* filmPositionRay) const;
	std::unique_ptr<Camera> clone() const;
};

