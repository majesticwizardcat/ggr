#include "accelerators/bounding-box.h"
#include "intersection/ray.h"
#include "tools/util.h"

#include <iostream>
#include <vector>

class Test {
public:
	BoundingBox b;
	Ray r;
	float expected;
	float result;

	Test(const BoundingBox& b, const Ray& r, float expected) : b(b), r(r),
		expected(expected) {
		result = b.intersects(r);
	}

	bool pass() const {
		return util::equals(expected, result);
	}
};

int main() {
	BoundingBox b0;
	b0.addPoint(Point3(-1.0f, -1.0f, -1.0f));
	b0.addPoint(Point3( 1.0f,  1.0f,  1.0f));
	std::vector<Test> tests;

	tests.push_back(Test(b0, Ray(Point3(0.0f), Vector3(1.0f, 0.0f, 0.0f)), 1.0f));
	tests.push_back(Test(b0, Ray(Point3(0.0f), Vector3(0.0f, 1.0f, 0.0f)), 1.0f));
	tests.push_back(Test(b0, Ray(Point3(0.0f), Vector3(0.0f, 0.0f, 1.0f)), 1.0f));
	tests.push_back(Test(b0, Ray(Point3(0.0f), Vector3(-1.0f, 0.0f, 0.0f)), 1.0f));
	tests.push_back(Test(b0, Ray(Point3(0.0f), Vector3(0.0f, -1.0f, 0.0f)), 1.0f));
	tests.push_back(Test(b0, Ray(Point3(0.0f), Vector3(0.0f, 0.0f, -1.0f)), 1.0f));
	tests.push_back(Test(b0, Ray(Point3(5.0f), Vector3(1.0f, 0.0f, 0.0f)), -1.0f));

	tests.push_back(Test(b0, Ray(Point3(0.5f, 2.0f, 0.5f), Vector3(1.0f, 0.0f, 0.0f)), -1.0f));
	tests.push_back(Test(b0, Ray(Point3(0.5f, 2.0f, 0.5f), Vector3(0.0f, 1.0f, 0.0f)), -1.0f));
	tests.push_back(Test(b0, Ray(Point3(0.5f, 2.0f, 0.5f), Vector3(0.0f, 0.0f, 1.0f)), -1.0f));
	tests.push_back(Test(b0, Ray(Point3(0.5f, 2.0f, 0.5f), Vector3(-1.0f, 0.0f, 0.0f)), -1.0f));
	tests.push_back(Test(b0, Ray(Point3(0.5f, 2.0f, 0.5f), Vector3(0.0f, -1.0f, 0.0f)), 1.0f));
	tests.push_back(Test(b0, Ray(Point3(0.5f, 2.0f, 0.5f), Vector3(0.0f, 0.0f, -1.0f)), -1.0f));

	tests.push_back(Test(b0, Ray(Point3(2.0f, 3.14f, -6.66f), Vector3(-0.4f, -0.45f, 1.0f)), 6.6066f));

	float t;
	for (int i = 0; i < tests.size(); ++i) {
		std::cout << "Running test" << i << ": ";
		if (tests[i].pass()) {
			std::cout << "PASS";
		}
		else {
			std::cout << "FAIL: result: " << tests[i].result
				<< " expected: " << tests[i].expected;;
		}
		std::cout << std::endl;
	}
}

