#pragma once

class StohasticSampler;

#include "samplers/sampler.h"
#include "samplers/sample-2d.h"
#include "tools/random-generator.h"

class StohasticSampler : public Sampler {
private:
	RandomGenerator m_rGen;

public:
	StohasticSampler() = delete;
	StohasticSampler(const Filter* filmFilter) : Sampler(filmFilter) { }
	StohasticSampler(const StohasticSampler& other) = delete;

	float getSample();
	Sample2D getSample2D();
	std::unique_ptr<Sampler> clone() const;
};
