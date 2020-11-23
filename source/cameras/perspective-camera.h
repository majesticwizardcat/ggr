#pragma once

class PerspectiveCamera;

#include "cameras/projective-camera.h"

class PerspectiveCamera : public ProjectiveCamera {
public:
	PerspectiveCamera() = delete;
	PerspectiveCamera(const PerspectiveCamera& other);
	PerspectiveCamera(const Transformation* cameraToWorld,
		int resolutionWidth, int resolutionHeight,
		float lensRadius, float focalDistance, float fov);

	void generateRay(Ray* ray, const CameraSample& sample) const;
	std::unique_ptr<Camera> clone() const;
};

