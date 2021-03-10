#pragma once

#include "spectra/spectrum.h"
#include "tools/constants.h"

namespace fresnel {
static const Spectrum ONE_SPECTRUM(1.0f);

inline Spectrum fresnelSchlick(const Spectrum& R0, float cosTheta) {
	float oct = 1.0f - cosTheta;
	return R0 + (ONE_SPECTRUM - R0) * (oct * oct * oct * oct * oct);
}

inline float fresnelCT(float eta, float cosTheta) {
	float gSqrd = eta * eta + cosTheta * cosTheta - 1.0f;
	if (gSqrd <= 0.0f) {
		return 1.0f;
	}
	float g = std::sqrt(gSqrd);
	float first = (g - cosTheta) / (g + cosTheta);
	first *= 0.5f * first;
	float denom = (g - cosTheta) * cosTheta + 1.0f;
	if (denom == 0.0f) {
		return 1.0f;
	}
	float sec = ((g + cosTheta) * cosTheta - 1.0f) / denom;
	sec *= sec;
	return first * (1.0f + sec);
}
}
