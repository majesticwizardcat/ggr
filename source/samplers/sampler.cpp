#include "samplers/sampler.h"
#include "tools/shading-functions.h"

Sampler::Sampler(float resolutionWidth, float resolutionHeight) :
	m_resolutionWidth(resolutionWidth), m_resolutionHeight(resolutionHeight) { }

CameraSample Sampler::createCameraSample(const Point2& rasterPosition) {
	Point2 filmPos = getSample2D() + rasterPosition;
	filmPos.x /= m_resolutionWidth;
	filmPos.y /= m_resolutionHeight;
	Point2 lesPos = shading::diskSample(getSample2D());
	return CameraSample(filmPos, lesPos);
}

CameraSample Sampler::getCameraSample(const Point2& rasterPosition) {
	if (m_samples.empty()) {
		return createCameraSample(rasterPosition);
	}
	CameraSample cm = m_samples.front();
	m_samples.pop();
	return cm;
}

void Sampler::createCameraSamples(const Point2& rasterPosition, int samples) {
	for (int i = 0; i < samples; ++i) {
		m_samples.push(createCameraSample(rasterPosition));
	}
}

