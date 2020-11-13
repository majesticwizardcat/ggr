#include "samplers/stohastic-sampler.h"

StohasticSampler::StohasticSampler(float resolutionWidth, float resolutionHeight) :
	Sampler(resolutionWidth, resolutionHeight) { }

float StohasticSampler::getSample() {
	return m_rGen.get();
}

Sample2D StohasticSampler::getSample2D() {
	return Sample2D(m_rGen.get(), m_rGen.get());
}

std::unique_ptr<Sampler> StohasticSampler::clone() const {
	return std::make_unique<StohasticSampler>(m_resolutionWidth, m_resolutionHeight);
}

std::unique_ptr<Sampler> StohasticSampler::clone(int resW, int resH) const {
	return std::make_unique<StohasticSampler>(resW, resH);
}

