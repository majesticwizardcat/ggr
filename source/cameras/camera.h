#pragma once

class Camera;

#include "primitives/transformation.h"
#include "intersection/ray.h"
#include "cameras/camera-sample.h"

#include <memory>

class Camera {
protected:
	std::shared_ptr<Transformation> m_cameraToWorld;
	float m_aspectRatio;
	float m_dx;
	float m_dy;

public:
	Camera() = delete;
	Camera(const Camera& other);
	Camera(const std::shared_ptr<Transformation>& cameraToWorld, int resolutionWidth,
		int resolutionHeight);

	virtual Ray generateRay(const CameraSample& sample) const = 0;
	virtual std::unique_ptr<Camera> clone() const = 0;
};

