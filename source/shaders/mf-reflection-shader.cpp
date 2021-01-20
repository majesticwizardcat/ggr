#include "shaders/mf-reflection-shader.h"
#include "shaders/microfacet-distribution.h"
#include "shaders/fresnel.h"

Spectrum MFReflectionShader::evaluate(const Vector3& wo, const Vector3& wi) const {
	float cosThetaWo = glm::dot(m_normal, wo);
	float cosThetaWi = glm::dot(m_normal, wi);
	float cosThetaWoWi = cosThetaWo * cosThetaWi;
	if (util::less(cosThetaWo, 0.0f)
		|| util::less(cosThetaWi, 0.0f)
		|| util::equals(cosThetaWoWi, 0.0f)) {
		return Spectrum(0.0f);
	}
	Vector3 m = glm::normalize(wo + wi);
	float MoWo = std::abs(glm::dot(m, wo)); // MoWi == MoWo since wi = reflect wo on m
	float D = mfdist::GGX_D(std::abs(glm::dot(m, m_normal)), m_alpha);
	float G = mfdist::GGX_G1(MoWo, cosThetaWo, m_alpha)
		* mfdist::GGX_G1(MoWo, cosThetaWi, m_alpha);
	return fresnel::fresnelSchlick(m_color, MoWo) * (D * G / (4.0f * cosThetaWoWi));
}

float MFReflectionShader::pdf(const Vector3& wo, const Vector3& wi) const {
	if (util::equals(glm::dot(m_normal, wo) * glm::dot(m_normal, wi), 0.0f)) {
		return 0.0f;
	}
	Vector3 m = glm::normalize(wo + wi);
	float MoWo = glm::dot(m, wo);
	if (util::less(MoWo, 0.0f)) {
		return 0.0f;
	}
	float cosThetaM = std::abs(glm::dot(m, m_normal));
	return mfdist::GGX_D(cosThetaM, m_alpha) * cosThetaM / (4.0f * MoWo);
}

Spectrum MFReflectionShader::sample(const Vector3& wo, Vector3* wi, float* pdf,
	bool* isDelta, Sampler* sampler) const {
	*isDelta = false;
	Vector3 m = mfdist::GGX_sampleNormal(m_alpha, sampler->getSample2D());
	toWorld(&m);
	float MoWo = glm::dot(m, wo);
	if (util::less(MoWo, 0.0f)) {
		*pdf = 0.0f;
		return Spectrum(0.0f);
	}
	*wi = shading::reflect(wo, m, MoWo);
	float cosThetaWo = glm::dot(m_normal, wo);
	float cosThetaWi = glm::dot(m_normal, *wi);
	float cosThetaWoWi = cosThetaWo * cosThetaWi;
	if (util::less(cosThetaWo, 0.0f)
		|| util::less(cosThetaWi, 0.0f)
		|| util::equals(cosThetaWoWi, 0.0f)) {
		*pdf = 0.0f;
		return Spectrum(0.0f);
	}
	float cosThetaM = std::abs(glm::dot(m, m_normal));
	float D = mfdist::GGX_D(cosThetaM, m_alpha);
	float G = mfdist::GGX_G1(MoWo, cosThetaWo, m_alpha)
		* mfdist::GGX_G1(MoWo, cosThetaWi, m_alpha);
	*pdf = D * cosThetaM / (4.0f * MoWo);
	return fresnel::fresnelSchlick(m_color, MoWo) * (G * D / (4.0f * cosThetaWoWi));
}
