#include "shaders/refraction-shader.h"
#include "shaders/fresnel.h"
#include "tools/util.h"
#include "tools/shading-functions.h"

Spectrum RefractionShader::sample(const Vector3& wo, Vector3* wi, float* pdf,
	bool* isDelta, Sampler* sampler) const {
	*isDelta = true;
	float cosTheta = glm::dot(m_normal, wo);
	Vector3 n = m_normal;
	if (util::equals(cosTheta, 0.0f)) {
		*pdf = 0.0f;
		return Spectrum(0.0f);
	}
	if (cosTheta < 0.0f) {
		cosTheta = -cosTheta;
		n = -n;
	}
	float F = fresnel::fresnelCT(m_eta, cosTheta);
	if (sampler->getSample() < F) {
		*pdf = F;
		*wi = shading::reflect(wo, n, cosTheta);
		return m_color * (F / cosTheta);
	}
	*wi = shading::refract(wo, n, 1.0f / m_eta, cosTheta);
	float cosThetaWi = std::abs(glm::dot(n, *wi));
	if (util::equals(cosThetaWi, 0.0f)) {
		*pdf = 0.0f;
		return Spectrum(0.0f);
	}
	F = 1.0f - F;
	*pdf = F;
	return m_color * (F * m_eta * m_eta / cosThetaWi);
}
