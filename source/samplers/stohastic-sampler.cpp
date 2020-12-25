#include "samplers/stohastic-sampler.h"

float StohasticSampler::getSample() {
	return m_rGen.get();
}

Sample2D StohasticSampler::getSample2D() {
	return Sample2D(m_rGen.get(), m_rGen.get());
}

std::unique_ptr<Sampler> StohasticSampler::clone() const {
	return std::make_unique<StohasticSampler>(m_filmFilter);
}
