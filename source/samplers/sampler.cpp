#include "samplers/sampler.h"
#include "tools/shading-functions.h"

CameraSample Sampler::getCameraSample(const Point2& rasterPosition) {
	if (m_samples.empty()) {
		return CameraSample(rasterPosition + m_filmFilter->sample(getSample2D()), getSample2D());
	}
	CameraSample cm = m_samples.back();
	m_samples.pop_back();
	return cm;
}

void Sampler::createCameraSamples(const Point2& rasterPosition, int samples) {
	for (int i = 0; i < samples; ++i) {
		m_samples.emplace_back(rasterPosition + m_filmFilter->sample(getSample2D()), getSample2D());
	}
}
