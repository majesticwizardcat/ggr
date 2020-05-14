#include "samplers/halton-sampler.h"
#include "tools/shading-functions.h"
#include "tools/primes.h"

HaltonSampler::HaltonSampler(const HaltonSampler& other) : Sampler(other) {
	initBases();
}

HaltonSampler::HaltonSampler(float resolutionWidth, float resolutionHeight) :
	Sampler(resolutionWidth, resolutionHeight) {
	initBases();
}

void HaltonSampler::initBases() {
	m_base0 = PRIMES[(int) (NUM_OF_PRIMES * m_rGen.get())];
	m_base1 = PRIMES[(int) (NUM_OF_PRIMES * m_rGen.get())];
	m_base2 = PRIMES[(int) (NUM_OF_PRIMES * m_rGen.get())];
	m_base3 = PRIMES[(int) (NUM_OF_PRIMES * m_rGen.get())];
	m_firstIndex = (m_base0 + m_base1 + m_base2 + m_base3) * m_rGen.get();
}

float HaltonSampler::sequence(int index, int base) const {
	float value = 1.0f;
	float result = 0.0f;
	while (index > 0) {
		value /= (float) base;
		result += value * (index % base);
		index /= base;
	}
	return result;
}

void HaltonSampler::createCameraSamples(const Point2& rasterPosition, int samples) {
	for (int i = m_firstIndex; i < m_firstIndex + samples; ++i) {
		Point2 filmPos = Point2(sequence(i, m_base0), sequence(i, m_base1)) + rasterPosition;
		filmPos.x /= m_resolutionWidth;
		filmPos.y /= m_resolutionHeight;
		Point2 lensPos = shading::diskSample(
			Sample2D(sequence(i, m_base2), sequence(i, m_base3)));
		m_samples.push(CameraSample(filmPos, lensPos));
	}
}

float HaltonSampler::getSample() {
	return m_rGen.get();
}

Sample2D HaltonSampler::getSample2D() {
	return Sample2D(m_rGen.get(), m_rGen.get());
}

std::unique_ptr<Sampler> HaltonSampler::clone() const {
	return std::make_unique<HaltonSampler>(*this);
}

std::unique_ptr<Sampler> HaltonSampler::clone(int resW, int resH) const {
	return std::make_unique<HaltonSampler>(resW, resH);
}

