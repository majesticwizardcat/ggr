#pragma once

class MicrofacetBTDF;

#include "bsdfs/bxdf.h"
#include "bsdfs/microfacet-distribution.h"
#include "bsdfs/fresnel.h"

class MicrofacetBTDF : public BXDF {
private:
	MicrofacetDistribution* m_D;
	Fresnel* m_fresnel;
	Spectrum m_color;
	float m_alpha;
	float m_eta;
	float m_IORin;
	float m_IORout;

public:
	MicrofacetBTDF() = delete;
	MicrofacetBTDF(const MicrofacetBTDF& other);
	MicrofacetBTDF(MicrofacetDistribution* D, Fresnel* fresnel, float alpha,
		const Spectrum& color, float IORin, float IORout);

	BXDFType getType() const;
	Spectrum evaluate(const Vector3& wo, const Vector3& wi) const;
	float pdf(const Vector3& wo, const Vector3& wi) const;
	BSDFSample sample(Sampler* sampler, const Vector3& wo) const;
	std::unique_ptr<BXDF> clone() const;
};

