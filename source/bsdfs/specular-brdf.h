#pragma once

class SpecularBRDF;

#include "bsdfs/bxdf.h"
#include "bsdfs/fresnel.h"
#include "spectra/spectrum.h"

#include <memory>

class SpecularBRDF : public BXDF {
private:
	Fresnel* m_fresnel;
	Spectrum m_color;

public:
	SpecularBRDF() = delete;
	SpecularBRDF(const SpecularBRDF& other);
	SpecularBRDF(const Spectrum& color, Fresnel* fresnel);

	BXDFType getType() const;
	Spectrum evaluate(const Vector3& wo, const Vector3& wi) const;
	float pdf(const Vector3& wo, const Vector3& wi) const;
	BSDFSample sample(Sampler* sampler, const Vector3& wo) const;
	std::unique_ptr<BXDF> clone() const;
};

