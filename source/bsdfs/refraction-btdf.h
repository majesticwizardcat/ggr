#pragma once

class RefractionBTDF;

#include "bsdfs/bxdf.h"
#include "bsdfs/fresnel.h"

#include <memory>

class RefractionBTDF : public BXDF {
private:
	Spectrum m_color;
	Fresnel* m_fresnel;
	float m_IORin;
	float m_IORout;
	float m_eta;

public:
	RefractionBTDF();
	RefractionBTDF(const RefractionBTDF& other);
	RefractionBTDF(const Spectrum& color, Fresnel* fresnel, float IORin, float IORout);

	BXDFType getType() const;
	Spectrum evaluate(const Vector3& wo, const Vector3& wi) const;
	float pdf(const Vector3& wo, const Vector3& wi) const;
	BSDFSample sample(Sampler* sampler, const Vector3& wo) const;
	std::unique_ptr<BXDF> clone() const;
};

