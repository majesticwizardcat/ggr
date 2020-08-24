#pragma once

class MicrofacetDistribution;
class GGXDistribution;

#include "primitives/vector.h"
#include "samplers/sampler.h"

class MicrofacetDistribution {
public:
	MicrofacetDistribution() { }
	virtual float D(const Vector3& m, const Vector3& wo, const Vector3& wi, float alpha) = 0;
	virtual float G1(const Vector3& m, const Vector3& w, float alpha) = 0;
	virtual Vector3 sampleNormal(Sampler* sampler, float alpha) = 0;
};


class GGXDistribution : public MicrofacetDistribution {
public:
	GGXDistribution();
	float D(const Vector3& m, const Vector3& wo, const Vector3& wi, float alpha);
	float G1(const Vector3& m, const Vector3& w, float alpha);
	Vector3 sampleNormal(Sampler* sampler, float alpha);
};

