#include "bsdfs/microfacet-brdf.h"
#include "tools/shading-functions.h"

MicrofacetBRDF::MicrofacetBRDF() { }
MicrofacetBRDF::MicrofacetBRDF(const MicrofacetBRDF& other) :
	MicrofacetBRDF(other.m_D, other.m_fresnel, other.m_alpha, other.m_color) { }
MicrofacetBRDF::MicrofacetBRDF(MicrofacetDistribution* D, Fresnel* fresnel, float alpha,
	const Spectrum& color) : m_D(D), m_fresnel(fresnel), m_alpha(alpha), m_color(color) { }

BXDFType MicrofacetBRDF::getType() const{
	return BXDFType::REFLECTION;
}

Spectrum MicrofacetBRDF::evaluate(const Vector3& wo, const Vector3& wi) const {
	if (shading::cosTheta(wo) * shading::cosTheta(wi) <= 0.0f) {
		return Spectrum(0.0f);
	}

	Normal m = (wo + wi).unit();
	float cosThetaWo = shading::absCosTheta(wo);
	float cosThetaWi = shading::absCosTheta(wi);
	float G = m_D->G1(m, wo, m_alpha) * m_D->G1(m, wi, m_alpha);
	float D = m_D->D(m, wo, wi, m_alpha);
	return m_color
		* ((m_fresnel->evaluate(1.5f, m.dot(wo)) * G * D) / (4.0f * cosThetaWo * cosThetaWi));
}

float MicrofacetBRDF::pdf(const Vector3& wo, const Vector3& wi) const {
	if (shading::cosTheta(wo) * shading::cosTheta(wi) <= 0.0f) {
		return 0.0f;
	}

	Normal m = (wo + wi).unit();
	float MoWo = m.dot(wo);
	if (MoWo == 0.0f) {
		return 0.0f;
	}

	return (m_D->D(m, wo, wi, m_alpha) * shading::absCosTheta(m)) / (4.0f * MoWo);
}

BSDFSample MicrofacetBRDF::sample(Sampler* sampler, const Vector3& wo) const {
	Normal m = m_D->sampleNormal(sampler, m_alpha);
	Vector3 wi = shading::reflect(wo, m);
	float cosThetaWi = shading::absCosTheta(wi);
	float cosThetaWo = shading::absCosTheta(wo);
	float MoWo = m.dot(wo);
	if (cosThetaWo == 0.0f || cosThetaWi == 0.0f || MoWo == 0.0f) {
		return BSDFSample();
	}

	Spectrum F = m_fresnel->evaluate(1.5f, MoWo);
	float G = m_D->G1(m, wo, m_alpha) * m_D->G1(m, wi, m_alpha);
	float D = m_D->D(m, wo, wi, m_alpha);
	return BSDFSample(m_color * ((F * G * D) / (4.0f * cosThetaWo * cosThetaWi)),
	       (D * shading::absCosTheta(m)) / (4.0f * MoWo),
	       wi,
	       false,
	       BXDFType::REFLECTION);
}

std::unique_ptr<BXDF> MicrofacetBRDF::clone() const {
	return std::unique_ptr<BXDF>(new MicrofacetBRDF(*this));
}

