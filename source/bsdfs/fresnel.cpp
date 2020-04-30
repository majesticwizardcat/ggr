#include "bsdfs/fresnel.h"

#include <math.h>

FresnelCT::FresnelCT() { }

Spectrum FresnelCT::evaluate(float eta, float cosTheta) const {
	float gSqrd = eta * eta + cosTheta * cosTheta - 1.0f;
	if (gSqrd <= 0.0f) {
		return Spectrum(1.0f);
	}

	float g = std::sqrt(gSqrd);
	float first = (g - cosTheta) / (g + cosTheta);
	first *= 0.5f * first;
	float denom = (g - cosTheta) * cosTheta + 1.0f;
	if (denom == 0.0f) {
		return Spectrum(1.0f);
	}

	float sec = ((g + cosTheta) * cosTheta - 1.0f) / denom;
	sec *= sec;
	return Spectrum(first * (1.0f + sec));
}

FresnelSchlick::FresnelSchlick() { }

FresnelSchlick::FresnelSchlick(const FresnelSchlick& other) : m_R0(other.m_R0),
	m_oneMinusR0(other.m_oneMinusR0) { }
FresnelSchlick::FresnelSchlick(const Spectrum& R0) : m_R0(R0), m_oneMinusR0(Spectrum(1.0f) - R0) { }

Spectrum FresnelSchlick::evaluate(float eta, float cosTheta) const {
	float oct = 1.0f - cosTheta;
	oct = oct * oct * oct * oct * oct;
	return m_R0 + m_oneMinusR0 * oct;
}

