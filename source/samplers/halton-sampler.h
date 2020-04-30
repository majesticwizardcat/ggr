#pragma once

class HaltonSampler;

#include "samplers/sampler.h"
#include "tools/random-generator.h"

const int FIRST_INDEX = 20;
const int BASE_0 = 2;
const int BASE_1 = 3;
const int BASE_2 = 5;
const int BASE_3 = 7;

class HaltonSampler : public Sampler {
private:
	RandomGenerator m_rGen;

	float sequence(int index, int base) const;

public:
	HaltonSampler() = delete;
	HaltonSampler(const HaltonSampler& other);
	HaltonSampler(float resolutionWidth, float resolutionHeight);

	void createCameraSamples(const Point2& rasterPosition, int samples);

	float getSample();
	Sample2D getSample2D();
	std::unique_ptr<Sampler> clone() const;
	std::unique_ptr<Sampler> clone(int resW, int resH) const;
};

