#pragma once

#include <random>
#include <chrono>

class RandomGenerator {
private:
	std::mt19937 m_randomDevice;
	std::uniform_real_distribution<float> m_distribution{ std::uniform_real_distribution<float>(0.0f, 0.9999f) };

public:
	RandomGenerator() noexcept :
		RandomGenerator(std::chrono::system_clock::now().time_since_epoch().count()) { }

	RandomGenerator(unsigned long long seed) : m_randomDevice(seed) { }

	inline float get() {
		return m_distribution(m_randomDevice);
	}
};
