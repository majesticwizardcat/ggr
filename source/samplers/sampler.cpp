#include "samplers/sampler.h"
#include "tools/shading-functions.h"

CameraSample Sampler::createCameraSample(const Point2& rasterPosition) {
	return CameraSample(rasterPosition + m_filmFilter->sample(getSample2D()), getSample2D());
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
