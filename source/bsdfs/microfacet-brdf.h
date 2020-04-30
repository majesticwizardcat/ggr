#pragma once

class MicrofacetBRDF;

#include "bsdfs/bxdf.h"
#include "bsdfs/fresnel.h"
#include "bsdfs/microfacet-distribution.h"

class MicrofacetBRDF : public BXDF {
private:
	MicrofacetDistribution* m_D;
	Fresnel* m_fresnel;
	float m_alpha;
	Spectrum m_color;

public:
	MicrofacetBRDF() = delete;
	MicrofacetBRDF(const MicrofacetBRDF& other);
	MicrofacetBRDF(MicrofacetDistribution* D, Fresnel* fresnel, float alpha,
		const Spectrum& color);

	BXDFType getType() const;
	Spectrum evaluate(const Vector3& wo, const Vector3& wi) const;
	float pdf(const Vector3& wo, const Vector3& wi) const;
	BSDFSample sample(Sampler* sampler, const Vector3& wo) const;
	std::unique_ptr<BXDF> clone() const;
};

