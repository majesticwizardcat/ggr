#include "bsdfs/diffuse-brdf.h"
#include "tools/constants.h"
#include "tools/shading-functions.h"

DiffuseBRDF::DiffuseBRDF() { }
DiffuseBRDF::DiffuseBRDF(const DiffuseBRDF& other) : m_diffuseColorOverPi(other.m_diffuseColorOverPi) { }
DiffuseBRDF::DiffuseBRDF(const Spectrum& diffuseColor) : m_diffuseColorOverPi(diffuseColor / PI) { }

BXDFType DiffuseBRDF::getType() const {
	return BXDFType::REFLECTION;
}

Spectrum DiffuseBRDF::evaluate(const Vector3& wo, const Vector3& wi) const {
	return shading::cosTheta(wo) * shading::cosTheta(wi) <= 0.0f
		? Spectrum(0.0f) : m_diffuseColorOverPi;
}

float DiffuseBRDF::pdf(const Vector3& wo, const Vector3& wi) const {
	return shading::cosTheta(wo) * shading::cosTheta(wi) <= 0.0f ? 0.0f : INV_TWO_PI;
}

BSDFSample DiffuseBRDF::sample(Sampler* sampler, const Vector3& wo) const {
	Vector3 sampledDir = shading::uniformSampleSphere(sampler->getSample2D());
	sampledDir.z = std::abs(sampledDir.z);
	return BSDFSample(m_diffuseColorOverPi, INV_TWO_PI, sampledDir, false, BXDFType::REFLECTION);
}

std::unique_ptr<BXDF> DiffuseBRDF::clone() const {
	return std::unique_ptr<BXDF>(new DiffuseBRDF(*this));
}

