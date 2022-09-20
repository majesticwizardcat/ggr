#include "cameras/camera.h"

Camera::Camera(const Camera& other) : m_cameraToWorld(other.m_cameraToWorld),
	m_dx(other.m_dx), m_dy(other.m_dy), m_rasterToFilm(other.m_rasterToFilm) { }

Camera::Camera(
	const Transformation* cameraToWorld, int resolutionWidth,
	int resolutionHeight) : m_cameraToWorld(cameraToWorld) 
{
	Transformation toFilm = transform::scale(1.0f / (float)(resolutionWidth),
		1.0f / (float)(resolutionHeight), 1.0f);
	Transformation centerFilm = transform::translate(-1.0f, 1.0f, 0.0f)
		* transform::scale(2.0f, -2.0f, 1.0f);
	m_rasterToFilm = centerFilm * toFilm;
}
