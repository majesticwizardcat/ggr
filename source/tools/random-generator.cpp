#include "tools/random-generator.h"

RandomGenerator::RandomGenerator() { 
	m_distribution = std::uniform_real_distribution<float>(0.0f, 0.9999f);
}

float RandomGenerator::get() {
	return m_distribution(m_randomEngine);
}

