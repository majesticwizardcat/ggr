#include "primitives/matrix.h"
#include "tools/util.h"

#include <iostream>

int main() {
	std::cout << "Running Matrix2 tests: " << std::endl;
	Matrix2 mat0(42, -1,
		     14, 82);
	Matrix2 mat1(6, 133,
		     -25, -68);

	std::cout << "Addition: ";
	if (mat0 + mat1 == Matrix2(42 + 6, -1 + 133,
					 14 - 25, 82 - 68)) {
		std::cout << "PASS";
	}
	else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Subtraction: ";
	if (mat0 - mat1 == Matrix2(42 - 6, -1 - 133,
					 14 + 25, 82 + 68)) {
		std::cout << "PASS";
	}
	else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Multiplication: ";
	if (mat0 * mat1 == Matrix2(277, 5654,
				     -1966, -3714)) {
		std::cout << "PASS";
	}
	else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Transpose: ";
	if (mat0.transpose() == Matrix2(42, 14,
					-1, 82)
		&& mat1.transpose() == Matrix2(6, -25,
					       133, -68)) {
		std::cout << "PASS";
	}
	else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Det: ";
	if (util::equals(mat0.det(), 3458)
		&& util::equals(mat1.det(), 2917)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Inverse: ";
	if (mat0.inverse() == Matrix2(41.0f / 1729.0f, 1.0f / 3458.0f,
				      -1.0f / 247.0f, 3.0f / 247.0f)
		&& mat1.inverse() == Matrix2(-68.0f / 2917.0f, -133.0f / 2917.0f,
					     25.0f / 2917.0f, 6.0f / 2917.0f)) {
		std::cout << "PASS";
	}
	else {
		std::cout << "FAIL";
	}
	std::cout << std::endl << std::endl;

	std::cout << "Running Matrix3 tests: " << std::endl;
	Matrix3 mat2(8, 3, -2,
		     4, 2, 1,
		     -2, -8, -9);
	Matrix3 mat3(4, 1, -1,
		     5, -9, -2,
		     -4, -7, 4);

	std::cout << "Addition: ";
	if (mat2 + mat3 == Matrix3(12, 4, -3,
				   9, -7, -1,
				   -6, -15, -5)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Subtraction: ";
	if (mat2 - mat3 == Matrix3(4, 2, -1,
				   -1, 11, 3,
				   2, -1, -13)) {
		std::cout << "PASS";
	}else{
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Multiplication: ";
	if (mat2 * mat3 == Matrix3(55, -5, -22,
				   22, -21, -4,
				   -12, 133, -18)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Transpose: ";
	if (mat2.transpose() == Matrix3(8, 4, -2,
				        3, 2, -8,
					-2, 1, -9)
		&& mat3.transpose() == Matrix3(4, 5, -4,
					       1, -9, -7,
					       -1, -2, 4)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Det: ";
	if (util::equals(mat2.det(), 78) && util::equals(mat3.det(), -141)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Inverse: ";
	if (mat2.inverse() == Matrix3(-0.128205f, 0.551282f, 0.089743f,
				      0.435897f, -0.974358f, -0.205128f,
				      -0.358974f, 0.743589f, 0.051282f)
		&& mat3.inverse() == Matrix3(0.354609f, -0.021276f, 0.078014f,
					     0.085106f, -0.085106f, -0.021276f,
					     0.503546f, -0.170212f, 0.290780f)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
	}
	std::cout << std::endl << std::endl;

	std::cout << "Running Matrix4 tests: " << std::endl;
	Matrix4 mat4(-1, -2, 9, 4,
		     8, 7, -3, 8,
		     -3, 3, 7, -4,
		     6, 2, -1, -5);
	Matrix4 mat5( 3, 4,  4,  5,
		      8, 0, -2, -2,
		     -8, 5, -5, -5,
		      1, 3,  5,  4);

	std::cout << "Addition: ";
	if (mat4 + mat5 == Matrix4(2, 2, 13, 9,
				   16, 7, -5, 6,
				   -11, 8, 2, -9,
				   7, 5, 4, -1)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Substraction: ";
	if (mat4 - mat5 == Matrix4(-4, -6, 5, -1,
				   0, 7, -1, 10,
				   5, -2, 12, 1,
				   5, -1, -6, -9)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Multiplication: ";
	if (mat4 * mat5 == Matrix4(-87, 53, -25, -30,
				   112, 41, 73, 73,
				   -45, 11, -73, -72,
				   37, 4, 0, 11)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
		std::cout << std::endl;
		(mat4 * mat5).print();
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Transpose: ";
	if (mat4.transpose() == Matrix4(-1, 8, -3, 6,
					-2, 7, 3, 2,
					9, -3, 7, -1,
					4, 8, -4, -5)
		&& mat5.transpose() == Matrix4(3,  8, -8, 1,
					       4,  0,  5, 3,
					       4, -2, -5, 5,
					       5, -2, -5, 4)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Det: ";
	if (util::equals(mat4.det(), -6109)
		&& util::equals(mat5.det(), -792)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;

	std::cout << "Inverse: ";
	if (mat4.inverse() == Matrix4(0.070387f, 0.011949f, -0.066950f, 0.128990f,
				      -0.082992f, 0.076608f, 0.132427f, -0.049762f,
				      0.085284f, -0.001800f, 0.037485f, 0.035357f,
				      0.034211f, 0.045342f, -0.034866f, -0.072188f)
		&& mat5.inverse() == Matrix4(0.012626f, 0.101010f, -0.017676f, 0.012626f,
					     0.070707f, 0.065656f, 0.101010f, 0.070707f,
					     -0.426767f, 0.085858f, -0.002525f, 0.573232f,
					     0.477272f, -0.181818f, -0.068181f, -0.522727f)) {
		std::cout << "PASS";
	}else {
		std::cout << "FAIL";
	}
	std::cout << std::endl;
}

