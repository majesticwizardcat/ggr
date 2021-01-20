#include "shaders/mf-refraction-shader.h"
#include "shaders/fresnel.h"
#include "shaders/microfacet-distribution.h"
#include "tools/util.h"
#include "tools/shading-functions.h"

Spectrum MFRefractionShader::evaluate(const Vector3& wo, const Vector3& wi) const {
	float cosThetaWo = glm::dot(m_normal, wo);
	float cosThetaWi = glm::dot(m_normal, wi);
	float cosThetaWoWi = cosThetaWo * cosThetaWi;
	if (util::equals(cosThetaWoWi, 0.0f)) {
		return Spectrum(0.0f);
	}
	cosThetaWo = std::abs(cosThetaWo);
	cosThetaWi = std::abs(cosThetaWi);
	if (cosThetaWoWi > 0.0f) {
		Vector3 m = glm::normalize(wo + wi);
		float MoWo = std::abs(glm::dot(m, wo)); // MoWi == MoWo since wi = reflect wo on m
		float D = mfdist::GGX_D(std::abs(glm::dot(m, m_normal)), m_alpha);
		float G = mfdist::GGX_G1(MoWo, cosThetaWo, m_alpha)
			* mfdist::GGX_G1(MoWo, cosThetaWi, m_alpha);
		float F = fresnel::fresnelCT(m_eta, MoWo);
		return m_color * (F * D * G / (4.0f * cosThetaWoWi));
	} 
	Vector3 m = glm::normalize(-(wi * m_IORout + wo * m_IORin));
	float MoWo = std::abs(glm::dot(m, wo));
	float MoWi = std::abs(glm::dot(m, wi));
	float denom = m_IORout * MoWo + m_IORin * MoWi;
	denom *= denom;
	if (util::equals(denom, 0.0f)) {
		return Spectrum(0.0f);
	}
	float F = 1.0f - fresnel::fresnelCT(m_eta, MoWo);
	if (util::equals(F, 0.0f)) {
		return Spectrum(0.0f);
	}
	float D = mfdist::GGX_D(std::abs(glm::dot(m, m_normal)), m_alpha);
	float G = mfdist::GGX_G1(MoWo, cosThetaWo, m_alpha)
		* mfdist::GGX_G1(MoWi, cosThetaWi, m_alpha);
	float first = (MoWo * MoWi) / (std::abs(cosThetaWoWi));
	return m_color * (((F * G * D * first * m_IORin * m_IORin) / denom) * (m_eta * m_eta));
}

float MFRefractionShader::pdf(const Vector3& wo, const Vector3& wi) const {
	float outside = glm::dot(m_normal, wo) * glm::dot(m_normal, wi);
	if (util::equals(outside, 0.0f)) {
		return 0.0f;
	}
	if (outside > 0.0f) {
		Vector3 m = glm::normalize(wo + wi);
		float MoWo = glm::dot(m, wo);
		if (util::less(MoWo, 0.0f)) {
			return 0.0f;
		}
		float cosThetaM = std::abs(glm::dot(m, m_normal));
		return mfdist::GGX_D(cosThetaM, m_alpha) * cosThetaM / (4.0f * MoWo);
	}
	Vector3 m = glm::normalize(-(wi * m_IORout + wo * m_IORin));
	float MoWo = std::abs(glm::dot(m, wo));
	float MoWi = std::abs(glm::dot(m, wi));
	float denom = m_IORout * MoWo + m_IORin * MoWi;
	denom *= denom;
	if (util::equals(denom, 0.0f)) {
		return 0.0f;
	}
	float cosThetaM = std::abs(glm::dot(m, m_normal));
	return mfdist::GGX_D(cosThetaM, m_alpha) * cosThetaM * m_IORin * m_IORin * MoWi / denom;
}

Spectrum MFRefractionShader::sample(const Vector3& wo, Vector3* wi, float* pdf,
	bool* isDelta, Sampler* sampler) const {
	*isDelta = false;
	float cosThetaWo = std::abs(glm::dot(m_normal, wo));
	if (util::equals(cosThetaWo, 0.0f)) {
		*pdf = 0.0f;
		return Spectrum(0.0f);
	}
	Vector3 m = mfdist::GGX_sampleNormal(m_alpha, sampler->getSample2D());
	toWorld(&m);
	float MoWo = glm::dot(m, wo);
	if (MoWo < 0.0f) {
		m = -m;
		MoWo = -MoWo;
	}
	float F = fresnel::fresnelCT(m_eta, MoWo);
	if (sampler->getSample() < F) {
		*wi = shading::reflect(wo, m, MoWo);
		float cosThetaWi = std::abs(glm::dot(m_normal, *wi));
		float cosThetaWoWi = cosThetaWo * cosThetaWi;
		if (util::equals(cosThetaWoWi, 0.0f)) {
			*pdf = 0.0f;
			return Spectrum(0.0f);
		}
		float cosThetaM = std::abs(glm::dot(m, m_normal));
		float D = mfdist::GGX_D(cosThetaM, m_alpha);
		float G = mfdist::GGX_G1(MoWo, cosThetaWo, m_alpha)
			* mfdist::GGX_G1(MoWo, cosThetaWi, m_alpha);
		*pdf = F * D * cosThetaM / (4.0f * MoWo);
		return m_color* (F * D * G / (4.0f * cosThetaWoWi));
	}
	*wi = shading::refract(wo, m, 1.0f / m_eta, MoWo);
	float cosThetaWi = std::abs(glm::dot(m_normal, *wi));
	float cosThetaWoWi = cosThetaWo * cosThetaWi;
	if (util::equals(cosThetaWoWi, 0.0f)) {
		*pdf = 0.0f;
		return Spectrum(0.0f);
	}
	cosThetaWi = std::abs(cosThetaWi);
	float MoWi = glm::dot(m, *wi);
	float denom = m_IORout * MoWo + m_IORin * MoWi;
	denom *= denom;
	if (MoWi * MoWo > 0.0f || util::equals(denom, 0.0f)) {
		*pdf = 0.0f;
		return Spectrum(0.0f);
	}
	MoWi = std::abs(MoWi);
	float cosThetaM = std::abs(glm::dot(m, m_normal));
	float D = mfdist::GGX_D(cosThetaM, m_alpha);
	float G = mfdist::GGX_G1(MoWo, cosThetaWo, m_alpha)
		* mfdist::GGX_G1(MoWi, cosThetaWi, m_alpha);
	float first = MoWo * MoWi / std::abs(cosThetaWoWi);
	float ior2 = m_IORin * m_IORin;
	F = 1.0f - F;
	*pdf = F * D * cosThetaM * ior2 * MoWi / denom;
	return m_color * (((F * G * D * first * ior2) / denom) * (m_eta * m_eta));
}
