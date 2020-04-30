#include "samplers/stohastic-sampler.h"
#include "samplers/halton-sampler.h"

#include <iostream>

int main() {
	StohasticSampler stohastic(1.0f, 1.0f);
	HaltonSampler halton(1.0f, 1.0f);
	stohastic.createCameraSamples(Point2(0, 0), 10 * 10);
	halton.createCameraSamples(Point2(0, 0), 10 * 10);
	
	std::cout << "Stohastic: " << std::endl;
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			CameraSample sample = stohastic.getCameraSample(Point2(i, j));
			sample.print();
			std::cout << std::endl;
		}
	}

	std::cout << "Halton: " << std::endl;
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			CameraSample sample = halton.getCameraSample(Point2(i, j));
			sample.print();
			std::cout << std::endl;
		}
	}
}

