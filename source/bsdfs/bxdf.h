#pragma once

class BXDF;

enum BXDFType {
	NONE, REFLECTION, REFRACTION, BOTH, EMISSION
};

#include "bsdfs/bsdf-sample.h"
#include "samplers/sampler.h"

#include <memory>

class BXDF {
public:
	BXDF() { }
	virtual BXDFType getType() const = 0;
	virtual Spectrum evaluate(const Vector3& wo, const Vector3& wi) const = 0;
	virtual float pdf(const Vector3& wo, const Vector3& wi) const = 0;
	virtual BSDFSample sample(Sampler* sampler, const Vector3& wo) const = 0;
	virtual std::unique_ptr<BXDF> clone() const = 0;
};

