#pragma once

class RandomGenerator;

#include <random>

class RandomGenerator {
private:
	std::random_device m_randomEngine;
	std::uniform_real_distribution<float> m_distribution;

public:
	RandomGenerator();

	float get();
};

