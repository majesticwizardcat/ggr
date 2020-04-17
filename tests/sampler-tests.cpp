#include "samplers/stohastic-sampler.h"

#include <iostream>

int main() {
	StohasticSampler sampler(10.0f, 10.0f);
	
	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			CameraSample sample = sampler.getCameraSample(Point2(i, j));
			sample.print();
			std::cout << std::endl;
		}
	}
}

