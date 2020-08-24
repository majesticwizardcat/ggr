#include "bsdfs/specular-brdf.h"
#include "tools/shading-functions.h"

SpecularBRDF::SpecularBRDF(const SpecularBRDF& other) : m_color(other.m_color),
	m_fresnel(other.m_fresnel) { }
SpecularBRDF::SpecularBRDF(const Spectrum& color, Fresnel* fresnel) : m_color(color),
	m_fresnel(fresnel) { }

BXDFType SpecularBRDF::getType() const {
	return BXDFType::REFLECTION;
}

Spectrum SpecularBRDF::evaluate(const Vector3& wo, const Vector3& wi) const {
	return Spectrum(0.0f);
}

float SpecularBRDF::pdf(const Vector3& wo, const Vector3& wi) const {
	return 0.0f;
}

BSDFSample SpecularBRDF::sample(Sampler* sampler, const Vector3& wo) const {
	float cosTheta = shading::absCosTheta(wo);
	if (cosTheta == 0.0f) {
		return BSDFSample();
	}

	return BSDFSample((m_fresnel->evaluate(1.5f, cosTheta) * m_color) / cosTheta,
		1.0f,
		shading::reflect(wo, Vector3(0.0f, 0.0f, 1.0f)),
		true,
		BXDFType::REFLECTION);
}

std::unique_ptr<BXDF> SpecularBRDF::clone() const {
	return std::make_unique<SpecularBRDF>(*this);
}

