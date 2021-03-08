#pragma once

#include "primitives/vector.h"
#include "primitives/point.h"
#include "samplers/sample-2d.h"
#include "spectra/spectrum.h"
#include "tools/constants.h"

namespace shading {
inline Point3 uniformSampleSphere(const Sample2D& sample) {
	float phi = sample.s * TWO_PI;
	float cosTheta = 1.0f - 2.0f * sample.t;
	float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
	return Point3(sinTheta * std::cos(phi),
		sinTheta * std::sin(phi),
		cosTheta);
}

inline Point3 fromSpherical(float phi, float theta) {
	float sinTheta = std::sin(theta);
	return Point3(sinTheta * std::cos(phi),
			sinTheta * std::sin(phi),
			std::cos(theta));
}

inline Point2 diskSample(const Sample2D& sample) {
	float r = std::sqrt(sample.s);
	float phi = sample.t * TWO_PI;
	return Point2(r * std::cos(phi), r * std::sin(phi));
}

inline Point2 toSpherical(const Vector3& v) {
	if (v.x == 0.0f) {
		return Point2(PI, std::abs(std::acos(v.z)));
	}
	return Point2(2.0f * std::abs(std::atan(v.y / v.x)),
		std::abs(std::acos(v.z)));
}

inline Vector3 reflect(const Vector3& w, const Vector3& n, float NoW) {
	return n * 2.0f * NoW - w;
}

inline Vector3 refract(const Vector3& w, const Vector3& n, float oneOverEta, float cosThetaW) {
	float sin2ThetaEta2 = oneOverEta * oneOverEta * (1.0f - cosThetaW * cosThetaW);
	if (sin2ThetaEta2 > 1.0f) {
		return Vector3(0.0f);
	}
	float cosThetaT = std::sqrt(1.0f - sin2ThetaEta2);
	return -w * oneOverEta + n * (oneOverEta * cosThetaW - cosThetaT);
}
}
