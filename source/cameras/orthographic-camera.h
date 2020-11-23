#pragma once

class OrthographicCamera;

#include "cameras/projective-camera.h"

class OrthographicCamera : public ProjectiveCamera {
public:
	OrthographicCamera() = delete;
	OrthographicCamera(const OrthographicCamera& other);
	OrthographicCamera(const Transformation* cameraToWorld,
		int resolutionWidth, int resolutionHeight,
		float lensRadius, float focalDistance, float scale);

	void generateRay(Ray* ray, const CameraSample& sample) const;
	std::unique_ptr<Camera> clone() const;
};

