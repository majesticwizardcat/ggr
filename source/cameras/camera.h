#pragma once

class Camera;

#include "primitives/transformation.h"
#include "intersection/ray.h"
#include "cameras/camera-sample.h"

#include <memory>

class Camera {
protected:
	const Transformation* m_cameraToWorld;
	Transformation m_rasterToFilm;
	Point3 m_dx;
	Point3 m_dy;

public:
	Camera() = delete;
	Camera(const Camera& other);
	Camera(const Transformation* cameraToWorld, int resolutionWidth, int resolutionHeight);

	virtual void generateRay(Ray* ray, const CameraSample& sample) const = 0;
	virtual std::unique_ptr<Camera> clone() const = 0;
};

