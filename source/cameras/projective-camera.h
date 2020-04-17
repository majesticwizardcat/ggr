#pragma once

class ProjectiveCamera;

#include "cameras/camera.h"

class ProjectiveCamera : public Camera {
private:
	Ray applyDOF(const Ray& ray, const Point2& lensPosition) const;

protected:
	std::shared_ptr<Transformation> m_projection;
	float m_lensRadius;
	float m_focalDistance;

public:
	ProjectiveCamera();
	ProjectiveCamera(const ProjectiveCamera& other);
	ProjectiveCamera(const std::shared_ptr<Transformation>&  cameraToWorld,
		int resolutionWidth, int resolutionHeight,
		const std::shared_ptr<Transformation>& projection,
		float lensRadius, float focalDistance);

	Ray generateRay(const CameraSample& sample) const;
	virtual Ray unproject(const Point2& filmPosition) const = 0;
	virtual std::unique_ptr<Camera> clone() const = 0;
};

