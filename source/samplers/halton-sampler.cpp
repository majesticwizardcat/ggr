#include "samplers/halton-sampler.h"
#include "tools/shading-functions.h"
#include "tools/primes.h"

void HaltonSampler::initBases() {
	m_base0 = PRIMES[(int) (NUM_OF_PRIMES * m_rGen.get())];
	m_base1 = PRIMES[(int) (NUM_OF_PRIMES * m_rGen.get())];
	m_base2 = PRIMES[(int) (NUM_OF_PRIMES * m_rGen.get())];
	m_base3 = PRIMES[(int) (NUM_OF_PRIMES * m_rGen.get())];
	m_firstIndex = 27;
}

float HaltonSampler::sequence(int index, int base) const {
	float value = 1.0f;
	float result = 0.0f;
	while (index > 0) {
		value /= (float) base;
		result += value * (float)(index % base);
		index /= base;
	}
	return result;
}

void HaltonSampler::createCameraSamples(const Point2& rasterPosition, int samples) {
	for (int i = m_firstIndex; i < m_firstIndex + samples; ++i) {
		m_samples.emplace_back(rasterPosition + Point2(sequence(i, m_base0), sequence(i, m_base1)),
			Sample2D(sequence(i, m_base2), sequence(i, m_base3)));
	}
	m_firstIndex += samples;
}

float HaltonSampler::getSample() {
	return m_rGen.get();
}

Sample2D HaltonSampler::getSample2D() {
	return Sample2D(m_rGen.get(), m_rGen.get());
}

std::unique_ptr<Sampler> HaltonSampler::clone() const {
	return std::make_unique<HaltonSampler>(m_filmFilter);
}
