#include "bsdfs/microfacet-distribution.h"
#include "tools/shading-functions.h"
#include "tools/constants.h"

#include <cmath>

GGXDistribution::GGXDistribution() { }

float GGXDistribution::D(const Vector3& m, const Vector3& wo, const Vector3& wi, float alpha) {
	float NoM = shading::absCosTheta(m);
	float NoM2 = NoM * NoM;
	if (NoM2 == 0.0f) {
		return 0.0f;
	}

	float alpha2 = alpha * alpha;
	float tan2ThetaM = (1.0f - NoM2) / NoM2;
	float cosThetaM4 = NoM2 * NoM2;
	float aTan2ThetaM2 = alpha2 + tan2ThetaM;
	aTan2ThetaM2 *= aTan2ThetaM2;
	float denom = PI * cosThetaM4 * aTan2ThetaM2;
	if (denom == 0.0f) {
		return 0.0f;
	}

	return (alpha2 * NoM) / denom;
}

float GGXDistribution::G1(const Vector3& m, const Vector3& w, float alpha) {
	float cosThetaV = shading::absCosTheta(w);
	float VoMOverVoN = std::abs(glm::dot(m, w) / cosThetaV);
	float alpha2 = alpha * alpha;
	float tan2ThetaV = (1.0f - cosThetaV * cosThetaV) / (cosThetaV * cosThetaV);
	return 2.0f / (1.0f + std::sqrt(1.0f + alpha2 * tan2ThetaV));
}

Vector3 GGXDistribution::sampleNormal(Sampler* sampler, float alpha) {
	Sample2D u = sampler->getSample2D();
	float theta = std::atan((alpha * std::sqrt(u.s)) / std::sqrt(1.0f - u.s));
	float phi = TWO_PI * u.t;
	Vector3 m = shading::fromSpherical(phi, theta);
	m.z = std::abs(m.z);
	return m;
}

