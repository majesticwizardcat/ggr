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
	StohasticSampler(const StohasticSampler& other) = delete;
	StohasticSampler(float resolutionWidth, float resolutionHeight);

	float getSample();
	Sample2D getSample2D();
	std::unique_ptr<Sampler> clone() const;
	std::unique_ptr<Sampler> clone(int resW, int resH) const;
};

