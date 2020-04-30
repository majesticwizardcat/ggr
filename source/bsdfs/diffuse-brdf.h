#pragma once

class DiffuseBRDF;

#include "bsdfs/bxdf.h"

#include <memory>

class DiffuseBRDF : public BXDF {
private:
	Spectrum m_diffuseColorOverPi;

public:
	DiffuseBRDF() = delete;
	DiffuseBRDF(const DiffuseBRDF& other);
	DiffuseBRDF(const Spectrum& diffuseColor);

	BXDFType getType() const;
	Spectrum evaluate(const Vector3& wo, const Vector3& wi) const;
	float pdf(const Vector3& wo, const Vector3& wi) const;
	BSDFSample sample(Sampler* sampler, const Vector3& wo) const;
	std::unique_ptr<BXDF> clone() const;
};

