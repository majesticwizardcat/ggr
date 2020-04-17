#include "tools/random-generator.h"

RandomGenerator::RandomGenerator() { 
	m_distribution = std::uniform_real_distribution<float>(0.0f, 1.0f);
}

float RandomGenerator::get() {
	return m_distribution(m_randomEngine);
}

