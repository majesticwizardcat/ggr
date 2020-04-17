#include "bsdfs/microfacet-btdf.h"
#include "tools/shading-functions.h"

#include <algorithm>

MicrofacetBTDF::MicrofacetBTDF() : m_alpha(0.0f), m_eta(0.0f), m_IORin(0.0f),
	m_IORout(0.0f) { }
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
		Normal m = (wo + wi).unit();
		float cosThetaWo = shading::absCosTheta(wo);
		float cosThetaWi = shading::absCosTheta(wi);
		float G = m_D->G1(m, wo, m_alpha) * m_D->G1(m, wi, m_alpha);
		float D = m_D->D(m, wo, wi, m_alpha);
		return (m_fresnel->evaluate(m_eta, m.dot(wo)) * G * D) / (4.0f * cosThetaWo * cosThetaWi);
	} 

	Normal m = -(wi * m_IORout + wo * m_IORin).unit();
	float MoWo = std::abs(m.dot(wo));
	float MoWi = std::abs(m.dot(wi));
	float cosThetaWo = shading::absCosTheta(wo);
	float cosThetaWi = shading::absCosTheta(wi);
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
	float first = (MoWo * MoWi) / (cosThetaWo * cosThetaWi);
	return ((m_color * F * G * D * first * m_IORin * m_IORin) / denom) / (m_eta * m_eta);
}

float MicrofacetBTDF::pdf(const Vector3& wo, const Vector3& wi) const {
	float outside = shading::cosTheta(wo) * shading::cosTheta(wi);
	if (outside == 0.0f) {
		return 0.0f;
	}

	if (outside > 0.0f) {
		Normal m = (wo + wi).unit();
		float MoWo = m.dot(wo);
		if (MoWo == 0.0f) {
			return 0.0f;
		}

		return (m_D->D(m, wo, wi, m_alpha) * shading::absCosTheta(m)) / (4.0f * MoWo);
	}

	Normal m = -(wi * m_IORout + wo * m_IORin).unit();
	float MoWo = std::abs(m.dot(wo));
	float MoWi = std::abs(m.dot(wi));
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

	Normal m = m_D->sampleNormal(sampler, m_alpha);
	if (m.dot(wo) < 0) {
		m = -m;
	}

	float MoWo = std::abs(m.dot(wo));
	Spectrum F = m_fresnel->evaluate(m_eta, MoWo);
	float choice = F.value();

	if (sampler->getSample() < choice) {
		Vector3 wi = shading::reflect(wo, m);
		float cosThetaWi = shading::absCosTheta(wi);
		if (cosThetaWi == 0.0f) {
			return BSDFSample();
		}

		float MoWo = m.dot(wo);
		Spectrum F = m_fresnel->evaluate(m_eta, MoWo);
		float G = m_D->G1(m, wo, m_alpha) * m_D->G1(m, wi, m_alpha);
		float D = m_D->D(m, wo, wi, m_alpha);
		return BSDFSample((F * G * D) / (4.0f * cosThetaWo * cosThetaWi),
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

	float MoWi = std::abs(m.dot(wi));
	float denom = m_IORout * MoWo + m_IORin * MoWi;
	denom *= denom;

	if (denom == 0.0f) {
		return BSDFSample();
	}

	float G = m_D->G1(m, wo, m_alpha) * m_D->G1(m, wi, m_alpha);
	float D = m_D->D(m, wo, wi, m_alpha);
	float first = (MoWo * MoWi) / (cosThetaWo * cosThetaWi);
	float IORin2 = m_IORin * m_IORin;
	Spectrum value = (m_color * (Spectrum(1.0f) - F) * G * D * first * IORin2) / denom;
	return BSDFSample(
		value / (m_eta * m_eta),
		(D * shading::absCosTheta(m) * IORin2 * MoWi * (1.0f - choice)) / denom,
		wi,
		false,
		BXDFType::REFRACTION);
}

std::unique_ptr<BXDF> MicrofacetBTDF::clone() const {
	return std::unique_ptr<BXDF>(new MicrofacetBTDF(*this));
}

