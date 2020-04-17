#include "cameras/perspective-camera.h"
#include "cameras/orthographic-camera.h"
#include "tools/constants.h"

#include <memory>
#include <iostream>

int main() {
	std::shared_ptr<Transformation>
		ctw(new Transformation(transform::lookAt(Point3(0.0f, 0.0f, 2.0f), Point3(0.0f), Vector3(0.0f, 1.0f, 0.0f))));
	PerspectiveCamera perspective(ctw, 10, 10, 0.0f, 0.0f, PI_OVER_TWO);
	OrthographicCamera orthographic(ctw, 10, 10, 0.0f, 0.0f, 1.0f);

	std::cout << "Testing ray generation:" << std::endl;
	for (float y = 0.0f; y <= 1.0f; y += 0.1f) {
		for (float x = 0.0f; x <= 1.0f; x += 0.1f) {
			std::cout << "X: " << x << " Y: " << y << " -> ";
			CameraSample sample(Point2(x, y), Point2(0.0f));
			Ray pers = perspective.generateRay(sample);
			Ray orth = orthographic.generateRay(sample);

			std::cout << " Perspective: ";
			if (pers == Ray(Point3(0.0f, 0.0f, 2.0f), (Vector3(2.0f * x - 1.0f, 1.0f - 2.0f * y, -1.0f)).unit())) {
				std::cout << "PASS";
			}
			else {
				std::cout << "FAIL" << std::endl;
				pers.print();
				std::cout << " Expected:" << std::endl;
				(Ray(Point3(0.0f, 0.0f, 2.0f), (Vector3(2.0f * x - 1.0f, 1.0f - 2.0f * y, -1.0f)).unit())).print();
			}
			std::cout << ", Orthographic: ";

			if (orth == Ray(Point3(2.0f * x - 1.0f, 1.0f - 2.0f * y, 2.0f), Vector3(0.0f, 0.0f, -1.0f))) {
				std::cout << "PASS";
			}
			else {
				std::cout << "FAIL";
			}
			std::cout << std::endl;
		}
	}

}

