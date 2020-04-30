#pragma once

class OrthographicCamera;

#include "cameras/projective-camera.h"

class OrthographicCamera : public ProjectiveCamera {
private:
	float m_scale;

public:
	OrthographicCamera() = delete;
	OrthographicCamera(const OrthographicCamera& other);
	OrthographicCamera(const std::shared_ptr<Transformation>& cameraToWorld,
		int resolutionWidth, int resolutionHeight,
		float lensRadius, float focalDistance, float scale);

	Ray unproject(const Point2& filmPosition) const;
	std::unique_ptr<Camera> clone() const;
};

