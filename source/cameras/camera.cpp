#include "cameras/camera.h"

Camera::Camera(const Camera& other) : m_cameraToWorld(other.m_cameraToWorld),
	m_dx(other.m_dx), m_dy(other.m_dy) { }
Camera::Camera(const std::shared_ptr<Transformation>& cameraToWorld, int resolutionWidth,
	int resolutionHeight) : m_cameraToWorld(cameraToWorld) {
	m_dx = 1.0f / ((float) resolutionWidth);
	m_dy = 1.0f / ((float) resolutionHeight);
}
