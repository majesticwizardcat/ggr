#include "bsdfs/emission-bxdf.h"

EmissionBXDF::EmissionBXDF() : BXDF() { }
EmissionBXDF::EmissionBXDF(const EmissionBXDF& other) : BXDF(other), m_emission(other.m_emission) { }
EmissionBXDF::EmissionBXDF(const Spectrum& emission) : BXDF(), m_emission(emission) { }

BXDFType EmissionBXDF::getType() const {
	return BXDFType::EMISSION;
}

Spectrum EmissionBXDF::evaluate(const Vector3& wo, const Vector3& wi) const {
	return wo.z > 0 ? m_emission : Spectrum(0.0f);
}

float EmissionBXDF::pdf(const Vector3& wo, const Vector3& wi) const {
	return 0.0f;
}

BSDFSample EmissionBXDF::sample(Sampler* sampler, const Vector3& wo) const {
	return BSDFSample(m_emission, 0.0f, Vector3(0.0f), true, BXDFType::EMISSION);
}

std::unique_ptr<BXDF> EmissionBXDF::clone() const {
	return std::unique_ptr<BXDF>(new EmissionBXDF(*this));
}

