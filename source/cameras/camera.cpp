#include "cameras/camera.h"

Camera::Camera(const Camera& other) : m_cameraToWorld(other.m_cameraToWorld),
	m_dx(other.m_dx), m_dy(other.m_dy) { }
Camera::Camera(const Transformation* cameraToWorld, int resolutionWidth,
	int resolutionHeight) : m_cameraToWorld(cameraToWorld) { }
