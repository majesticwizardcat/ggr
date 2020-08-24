#include "bsdfs/microfacet-btdf.h"
#include "tools/shading-functions.h"

#include <algorithm>

MicrofacetBTDF::MicrofacetBTDF(const MicrofacetBTDF& other) :
	MicrofacetBTDF(other.m_D, other.m_fresnel, other.m_alpha, other.m_color,
	other.m_IORin, other.m_IORout) { }
MicrofacetBTDF::MicrofacetBTDF(MicrofacetDistribution* D, Fresnel* fresnel, float alpha,
	const Spectrum& color, float IORin, float IORout) : m_D(D), m_fresnel(fresnel),
	m_alpha(alpha), m_color(color), m_IORin(IORin), m_IORout(IORout) {
	m_eta = m_IORout / m_IORin;
}

BXDFType MicrofacetBTDF::getType() const {
	return BXDFType::BOTH;
}

Spectrum MicrofacetBTDF::evaluate(const Vector3& wo, const Vector3& wi) const {
	float outside = shading::cosTheta(wo) * shading::cosTheta(wi);
	if (outside == 0.0f) {
		return Spectrum(0.0f);
	}

	if (outside > 0.0f) {
		Vector3 m = glm::normalize(wo + wi);
		float G = m_D->G1(m, wo, m_alpha) * m_D->G1(m, wi, m_alpha);
		float D = m_D->D(m, wo, wi, m_alpha);
		float r = (G * D) / (4.0f * outside);
		return m_fresnel->evaluate(m_eta, glm::dot(m, wo)) * r;
	} 

	Vector3 m = glm::normalize(-(wi * m_IORout + wo * m_IORin));
	float MoWo = std::abs(glm::dot(m, wo));
	float MoWi = std::abs(glm::dot(m, wi));
	float denom = m_IORout * MoWo + m_IORin * MoWi;
	denom *= denom;
	if (denom == 0.0f) {
		return Spectrum(0.0f);
	}

	Spectrum F = Spectrum(1.0f) - m_fresnel->evaluate(m_eta, MoWo);
	if (F.isZero()) {
		return Spectrum(0.0f);
	}

	float G = m_D->G1(m, wo, m_alpha) * m_D->G1(m, wi, m_alpha);
	float D = m_D->D(m, wo, wi, m_alpha);
	float first = (MoWo * MoWi) / (std::abs(outside));
	float r = ((G * D * first * m_IORin * m_IORin) / denom) * (m_eta * m_eta);
	return m_color * F * r;
}

float MicrofacetBTDF::pdf(const Vector3& wo, const Vector3& wi) const {
	float outside = shading::cosTheta(wo) * shading::cosTheta(wi);
	if (outside == 0.0f) {
		return 0.0f;
	}

	if (outside > 0.0f) {
		Vector3 m = glm::normalize(wo + wi);
		float MoWo = glm::dot(m, wo);
		if (MoWo == 0.0f) {
			return 0.0f;
		}

		return (m_D->D(m, wo, wi, m_alpha) * shading::absCosTheta(m)) / (4.0f * MoWo);
	}

	Vector3 m = glm::normalize(-(wi * m_IORout + wo * m_IORin));
	float MoWo = std::abs(glm::dot(m, wo));
	float MoWi = std::abs(glm::dot(m, wi));
	float denom = m_IORout * MoWo + m_IORin * MoWi;
	denom *= denom;
	if (denom == 0.0f) {
		return 0.0f;
	}

	return (m_D->D(m, wo, wi, m_alpha) * shading::absCosTheta(m) * m_IORin * m_IORin * MoWi) / denom;
}

BSDFSample MicrofacetBTDF::sample(Sampler* sampler, const Vector3& wo) const {
	float cosThetaWo = shading::absCosTheta(wo);
	if (cosThetaWo == 0.0f) {
		return BSDFSample();
	}

	Vector3 m = m_D->sampleNormal(sampler, m_alpha);
	float MoWo = glm::dot(m, wo);
	if (MoWo < 0) {
		m = -m;
	}

	MoWo = std::abs(MoWo);
	Spectrum F = m_fresnel->evaluate(m_eta, MoWo);
	float choice = F.value();

	if (sampler->getSample() < choice) {
		Vector3 wi = shading::reflect(wo, m);
		float cosThetaWi = shading::absCosTheta(wi);
		if (cosThetaWi == 0.0f) {
			return BSDFSample();
		}

		Spectrum F = m_fresnel->evaluate(m_eta, MoWo);
		float G = m_D->G1(m, wo, m_alpha) * m_D->G1(m, wi, m_alpha);
		float D = m_D->D(m, wo, wi, m_alpha);
		float r = G * D / (4.0f * cosThetaWo * cosThetaWi);
		return BSDFSample(F * r,
		       (D * shading::absCosTheta(m) * choice) / (4.0f * MoWo),
		       wi,
		       false,
		       BXDFType::REFLECTION);
	}

	Vector3 wi = shading::refract(wo, m, m_IORin, m_IORout);
	float cosThetaWi = shading::absCosTheta(wi);
	if (cosThetaWi == 0.0f) {
		return BSDFSample();
	}

	float MoWi = std::abs(glm::dot(m, wi));
	float denom = m_IORout * MoWo + m_IORin * MoWi;
	denom *= denom;

	if (denom == 0.0f) {
		return BSDFSample();
	}

	float G = m_D->G1(m, wo, m_alpha) * m_D->G1(m, wi, m_alpha);
	float D = m_D->D(m, wo, wi, m_alpha);
	float first = (MoWo * MoWi) / (cosThetaWo * cosThetaWi);
	float IORin2 = m_IORin * m_IORin;
	float r = ((G * D * first * IORin2) / denom) * (m_eta * m_eta);
	Spectrum value = (m_color * ((Spectrum(1.0f) - F))) * r;
	return BSDFSample(
		value,
		(D * shading::absCosTheta(m) * IORin2 * MoWi * (1.0f - choice)) / denom,
		wi,
		false,
		BXDFType::REFRACTION);
}

std::unique_ptr<BXDF> MicrofacetBTDF::clone() const {
	return std::make_unique<MicrofacetBTDF>(*this);
}

