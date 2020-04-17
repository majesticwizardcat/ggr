#include "cameras/camera.h"

Camera::Camera() : m_cameraToWorld(new Transformation()), m_aspectRatio(0.0f),
	m_dx(0.0f), m_dy(0.0f) { }
Camera::Camera(const Camera& other) : m_cameraToWorld(other.m_cameraToWorld),
	m_aspectRatio(other.m_aspectRatio), m_dx(other.m_dx), m_dy(other.m_dy) { }
Camera::Camera(const std::shared_ptr<Transformation>& cameraToWorld, int resolutionWidth,
	int resolutionHeight) : m_cameraToWorld(cameraToWorld) {
	m_aspectRatio = (float) ((float) resolutionWidth / (float) resolutionHeight);
	m_dx = 1.0f / ((float) resolutionWidth);
	m_dy = 1.0f / ((float) resolutionHeight);
}

