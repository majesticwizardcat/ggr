#pragma once

class HaltonSampler;

#include "samplers/sampler.h"
#include "tools/random-generator.h"

class HaltonSampler : public Sampler {
private:
	int m_base0;
	int m_base1;
	int m_base2;
	int m_base3;
	int m_firstIndex;
	RandomGenerator m_rGen;

	float sequence(int index, int base) const;
	void initBases();

public:
	HaltonSampler() = delete;
	HaltonSampler(const HaltonSampler& other) = delete;
	HaltonSampler(const Filter* filmFilter) : Sampler(filmFilter) {
		initBases();
	}

	void createCameraSamples(const Point2& rasterPosition, int samples);

	float getSample();
	Sample2D getSample2D();
	std::unique_ptr<Sampler> clone() const;
};
