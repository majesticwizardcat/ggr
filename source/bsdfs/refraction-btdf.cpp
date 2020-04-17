#include "bsdfs/refraction-btdf.h"
#include "tools/shading-functions.h"

RefractionBTDF::RefractionBTDF() : m_IORin(0.0f), m_IORout(0.0f), m_eta(0.0f) { }
RefractionBTDF::RefractionBTDF(const RefractionBTDF& other) : m_color(other.m_color),
	m_fresnel(other.m_fresnel), m_IORin(other.m_IORin), m_IORout(other.m_IORout), m_eta(other.m_eta) { }
RefractionBTDF::RefractionBTDF(const Spectrum& color, Fresnel* fresnel, float IORin, float IORout) :
	m_color(color), m_fresnel(fresnel), m_IORin(IORin), m_IORout(IORout) {
	m_eta = IORout / IORin;
}

BXDFType RefractionBTDF::getType() const {
	return BXDFType::BOTH;
}

Spectrum RefractionBTDF::evaluate(const Vector3& wo, const Vector3& wi) const {
	return Spectrum(0.0f);
}

float RefractionBTDF::pdf(const Vector3& wo, const Vector3& wi) const {
	return 0.0f;
}

BSDFSample RefractionBTDF::sample(Sampler* sampler, const Vector3& wo) const {
	float cosThetaWo = shading::cosTheta(wo);
	if (cosThetaWo == 0.0f) {
		return BSDFSample();
	}

	Normal n(0.0f, 0.0f, 1.0f);
	if (cosThetaWo < 0) {
		n.z = -1.0f;
		cosThetaWo = -cosThetaWo;
	}

	Spectrum fresnel = m_fresnel->evaluate(m_eta, cosThetaWo);
	float pdf = fresnel.value();

	if (sampler->getSample() < pdf) {
		return BSDFSample(fresnel / cosThetaWo,
			pdf,
			shading::reflect(wo, n),
			true,
			BXDFType::REFLECTION);
	}

	Vector3 sampledDirection = shading::refract(wo, n, m_IORin, m_IORout);
	float cosThetaWl = shading::absCosTheta(sampledDirection);
	if (cosThetaWl == 0.0f) {
		return BSDFSample();
	}

	return BSDFSample(((m_color * (Spectrum(1.0) - fresnel)) / (m_eta * m_eta)) / cosThetaWl,
		1.0f - pdf,
		sampledDirection,
		true,
		BXDFType::REFRACTION);
}

std::unique_ptr<BXDF> RefractionBTDF::clone() const {
	return std::unique_ptr<BXDF>(new RefractionBTDF(*this));
}

