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
	float r = sample.s;
	float phi = sample.t * TWO_PI;
	return Point2(r * std::cos(phi), r * std::sin(phi));
}

inline Point2 toSpherical(const Vector3& v) {
	return Point2(2.0f * std::abs(std::atan(v.y / v.x)),
		std::abs(std::acos(v.z)));
}

inline bool sameHemisphere(const Vector3& v0, const Vector3& v1) {
	return v0.z * v1.z >= 0.0f;
}

inline Vector3 reflect(const Vector3& w, const Vector3& n) {
	return n * 2.0f * glm::dot(n, w) - w;
}

inline Vector3 refract(const Vector3& w, const Vector3& n, float IORin, float IORout) {
	float eta = IORin / IORout;
	float cosThetaI = glm::dot(n, w);
	float sin2ThetaEta2 = eta * eta * (1.0f - cosThetaI * cosThetaI);
	if (sin2ThetaEta2 > 1.0f) {
		return Vector3(0.0f);
	}
	float cosThetaT = std::sqrt(1.0f - sin2ThetaEta2);
	return -w * eta + n * (eta * cosThetaI - cosThetaT);
}

inline float cosTheta(const Vector3& w) {
	return w.z;
}

inline float cos2Theta(const Vector3& w) {
	return w.z * w.z;
}

inline float absCosTheta(const Vector3& w) {
	return std::abs(w.z);
}
}

