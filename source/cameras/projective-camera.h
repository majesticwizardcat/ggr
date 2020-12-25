#pragma once

class ProjectiveCamera;

#include "cameras/camera.h"

class ProjectiveCamera : public Camera {
protected:
	Transformation m_unprojection;
	float m_lensRadius;
	float m_focalDistance;

	void applyDOF(Ray* ray, const Point2& lensPosition) const;

public:
	ProjectiveCamera() = delete;
	ProjectiveCamera(const ProjectiveCamera& other);
	ProjectiveCamera(const Transformation*  cameraToWorld,
		int resolutionWidth, int resolutionHeight,
		const Transformation& projection, float lensRadius, float focalDistance);

	void generateRay(Ray* ray, const CameraSample& sample) const = 0;
	virtual std::unique_ptr<Camera> clone() const = 0;
};

