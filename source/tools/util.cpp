#include "tools/util.h"
#include "tools/constants.h"

#include <algorithm>
#include <math.h>

Vector3 util::minComponentsVector(const Vector3& v0, const Vector3& v1) { 
	return Vector3(std::min(v0.x, v1.x),
		std::min(v0.y, v1.y),
		std::min(v0.z, v1.z));
}

Vector3 util::maxComponentsVector(const Vector3& v0, const Vector3& v1) {
	return Vector3(std::max(v0.x, v1.x),
		std::max(v0.y, v1.y),
		std::max(v0.z, v1.z));
}

Point3 util::minComponentsPoint(const Point3& p0, const Point3& p1) {
	return Point3(std::min(p0.x, p1.x),
		std::min(p0.y, p1.y),
		std::min(p0.z, p1.z));
}

Point3 util::maxComponentsPoint(const Point3& p0, const Point3& p1) {
	return Point3(std::max(p0.x, p1.x),
		std::max(p0.y, p1.y),
		std::max(p0.z, p1.z));
}

float util::clamp(float v, float min, float max) {
	if (v < min) {
		return min;
	}

	if (v > max) {
		return max;
	}

	return v;
}

bool util::equals(float f0, float f1) {
	return f0 >= f1 - ERROR && f0 <= f1 + ERROR;
}

bool util::less(float f0, float f1) {
	return f0 < f1 + ERROR;
}

bool util::greater(float f0, float f1) {
	return f0 > f1 - ERROR;
}

