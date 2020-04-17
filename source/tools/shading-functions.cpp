#include "tools/shading-functions.h"
#include "constants.h"

#include <math.h>
#include <algorithm>

Point3 shading::uniformSampleSphere(const Sample2D& sample) {
	float phi = sample.u0 * TWO_PI;
	float theta = std::acos(sample.u1);
	return shading::fromSpherical(phi, theta);
}

Point3 shading::fromSpherical(float phi, float theta) {
	return Point3(std::sin(theta) * std::cos(phi),
			std::sin(theta) * std::sin(phi),
			std::cos(theta));
}

Point2 shading::diskSample(const Sample2D& sample) {
	float r = sample.u0;
	float phi = sample.u1 * TWO_PI;
	return Point2(r * std::cos(phi), r * std::sin(phi));
}

Point2 shading::toSpherical(const Vector3& v) {
	return Point2(2.0f * std::abs(std::atan(v.y / v.x)),
		std::abs(std::acos(v.z)));
}

bool shading::sameHemisphere(const Vector3& v0, const Vector3& v1) {
	return v0.z * v1.z >= 0.0f;
}

Vector3 shading::reflect(const Vector3& w, const Normal& n) {
	return (n * 2.0f * n.dot(w) - w).unit();
}

Vector3 shading::refract(const Vector3& w, const Normal& n, float IORin, float IORout) {
	float eta = IORin / IORout;
	float cosThetaI = n.dot(w);
	float sin2ThetaI = 1.0f - cosThetaI * cosThetaI;
	float sin2ThetaT = eta * eta * sin2ThetaI;
	if (sin2ThetaT > 1.0f) {
		return Vector3(0.0f);
	}

	float cosThetaT = std::sqrt(1.0f - sin2ThetaT);
	return (-w * eta + n * (eta * cosThetaI - cosThetaT)).unit();
}

float shading::cosTheta(const Vector3& w) {
	return w.z;
}

float shading::cos2Theta(const Vector3& w) {
	return w.z * w.z;
}

float shading::absCosTheta(const Vector3& w) {
	return std::abs(cosTheta(w));
}

