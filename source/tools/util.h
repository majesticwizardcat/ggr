#pragma once

#include "primitives/vector.h"
#include "primitives/point.h"
#include "tools/constants.h"

namespace util {
inline float clamp(float v, float min, float max) {
	if (v < min) {
		return min;
	}
	if (v > max) {
		return max;
	}
	return v;
}

inline bool equals(float f0, float f1) {
	return std::abs(f0 - f1) - ERROR < 0.0f;
}

inline bool less(float f0, float f1) {
	return f0 - f1 - ERROR < 0.0f;
}

inline bool greater(float f0, float f1) {
	return less(f1, f0);
}
}
