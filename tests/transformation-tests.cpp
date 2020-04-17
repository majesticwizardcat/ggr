#include "primitives/transformation.h"
#include "tools/util.h"
#include "tools/constants.h"

#include <iostream>

int main() {
	Vector4 vec4(2.2f, 1.4f, -1.2f, 0.0f);
	Point4 p4(2.2f, 1.4f, -1.2f, 1.0f);
	
	Vector3 v(1.5f, 4.0f, -2.0f);
	Normal n(3.0f, -1.0f, 0.25f);

	Transformation trs = transform::translate(1.0f, 2.0f, -1.0f);
	Transformation rot = transform::rotate(PI, PI_OVER_TWO, PI_OVER_THREE);
	Transformation scl = transform::scale(3.3f, 5.1f, -2.1f);
	Transformation com = (trs.combine(rot)).combine(scl);

	std::cout << "Running Vector4 tests: " << std::endl;

	std::cout << "Translation: ";
	if (trs.apply(vec4) == Vector4(2.2f, 1.4f, -1.2f, 0.0f)) {
		std::cout << "PASS";
	}
	else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Rotation: ";
	if (rot.apply(vec4) == Vector4(1.81244f, 0.33923f, -2.19999f, 0.0f)) {
		std::cout << "PASS";
	}
	else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Scale: ";
	if (scl.apply(vec4) == Vector4(3.3f * 2.2f, 5.1f * 1.4f, -2.1f * -1.2f, 0.0f)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
	}
	std::cout << std::endl << std::endl;

	std::cout << "Running Point4 tests: " << std::endl;

	std::cout << "Translation: ";
	if (trs.apply(p4) == Point4(3.2f, 3.4f, -2.2f, 1.0f)) {
		std::cout << "PASS";
	}
	else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Rotation: ";
	if (rot.apply(p4) == Point4(1.81244f, 0.33923f, -2.19999f, 1.0f)) {
		std::cout << "PASS";
	}
	else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Scale: ";
	if (scl.apply(p4) == Point4(3.3f * 2.2f, 5.1f * 1.4f, -2.1f * -1.2f, 1.0f)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
	}
	std::cout << std::endl << std::endl;

	std::cout << "Running Normal tests: " << std::endl;

	std::cout << "Translation: ";
	if (util::equals(trs.apply(n).dot(trs.apply(v)), 0.0f)) {
		std::cout << "PASS";
	}
	else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Rotation: ";
	if (util::equals(rot.apply(n).dot(rot.apply(v)), 0.0f)) {
		std::cout << "PASS";
	}
	else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Scale: ";
	if (util::equals(scl.apply(n).dot(scl.apply(v)), 0.0f)) {
		std::cout << "PASS";
	}
	else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Combination: ";
	if (util::equals(com.apply(n).dot(com.apply(v)), 0.0f)) {
		std::cout << "PASS";
	}
	else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;
}

