#include "samplers/sampler.h"
#include "tools/shading-functions.h"

Sampler::Sampler() { }
Sampler::Sampler(const Sampler& other) :
	Sampler(other.m_resolutionWidth, other.m_resolutionHeight) { }
Sampler::Sampler(float resolutionWidth, float resolutionHeight) :
	m_resolutionWidth(resolutionWidth), m_resolutionHeight(resolutionHeight) { }

CameraSample Sampler::getCameraSample(const Point2& rasterPosition) {
	Point2 filmPos = getSample2D() + rasterPosition;
	filmPos.x /= m_resolutionWidth;
	filmPos.y /= m_resolutionHeight;
	Point2 lesPos = shading::diskSample(getSample2D());
	return CameraSample(filmPos, lesPos);
}

