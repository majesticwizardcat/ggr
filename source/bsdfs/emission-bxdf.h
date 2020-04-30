#pragma once

class EmissionBXDF;

#include "bsdfs/bxdf.h"

class EmissionBXDF : public BXDF {
private:
	Spectrum m_emission;

public:
	EmissionBXDF() = delete;
	EmissionBXDF(const EmissionBXDF& other);
	EmissionBXDF(const Spectrum& emisison);

	BXDFType getType() const;
	Spectrum evaluate(const Vector3& wo, const Vector3& wi) const;
	float pdf(const Vector3& wo, const Vector3& wi) const;
	BSDFSample sample(Sampler* sampler, const Vector3& wo) const;
	std::unique_ptr<BXDF> clone() const;
};

