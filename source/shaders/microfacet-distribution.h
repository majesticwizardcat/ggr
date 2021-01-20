#pragma once

#include "tools/constants.h"
#include "tools/util.h"
#include "tools/shading-functions.h"
#include "samplers/sampler.h"

namespace mfdist {
inline float GGX_D(float NoM, float alpha) {
	float NoM2 = NoM * NoM;
	float alpha2 = alpha * alpha;
	float tan2ThetaM = (1.0f - NoM2) / NoM2;
	float cosThetaM4 = NoM2 * NoM2;
	float aTan2ThetaM2 = alpha2 + tan2ThetaM;
	aTan2ThetaM2 *= aTan2ThetaM2;
	float denom = PI * cosThetaM4 * aTan2ThetaM2;
	if (denom == 0.0f) {
		return 0.0f;
	}
	return alpha2 / denom;
}

inline float GGX_G1(float MoW, float cosThetaW, float alpha) {
	float MoWOverNoW = std::abs(MoW / cosThetaW);
	float alpha2 = alpha * alpha;
	float tan2ThetaV = (1.0f - cosThetaW * cosThetaW) / (cosThetaW * cosThetaW);
	return 2.0f / (1.0f + std::sqrt(1.0f + alpha2 * tan2ThetaV));
}

inline Vector3 GGX_sampleNormal(float alpha, const Sample2D& u) {
	return shading::fromSpherical(TWO_PI * u.t,
		std::atan((alpha * std::sqrt(u.s)) / std::sqrt(1.0f - u.s)));
}
}
