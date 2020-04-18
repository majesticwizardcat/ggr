#include "materials/glass-material.h"
#include "bsdfs/microfacet-btdf.h"
#include "bsdfs/refraction-btdf.h"
#include "tools/util.h"

GlassMaterial::GlassMaterial() { }
GlassMaterial::GlassMaterial(const GlassMaterial& other) : m_color(other.m_color),
	m_roughness(other.m_roughness), m_materialIOR(other.m_materialIOR),
	m_airIOR(other.m_airIOR) {
	m_distribution = std::unique_ptr<MicrofacetDistribution>(new GGXDistribution());
	m_fresnel = std::unique_ptr<Fresnel>(new FresnelCT());
}

GlassMaterial::GlassMaterial(const std::shared_ptr<Texture>& color,
	const std::shared_ptr<Texture>& roughness, float materialIOR, float airIOR) :
	m_color(color), m_roughness(roughness), m_materialIOR(materialIOR), m_airIOR(airIOR) {
	m_distribution = std::unique_ptr<MicrofacetDistribution>(new GGXDistribution());
	m_fresnel = std::unique_ptr<Fresnel>(new FresnelCT());
}

GlassMaterial::GlassMaterial(const std::shared_ptr<Texture>& color,
	const std::shared_ptr<Texture>& roughness, float materialIOR) :
	GlassMaterial(color, roughness, materialIOR, 1.0f) { }

BSDF GlassMaterial::createBSDF(const SurfacePoint& point, const Vector3& wo) const {
	BSDF bsdf(point);
	float IORin;
	float IORout;
	if (point.geometricNormal.dot(wo) > 0) {
		IORin = m_airIOR;
		IORout = m_materialIOR;
	}
	else {
		IORin = m_materialIOR;
		IORout = m_airIOR;
	}

	Spectrum color = m_color->sample(point);
	float roughness = util::clamp(m_roughness->sample(point).value(), 0.0f, 1.0f);
	roughness *= roughness;

	if (util::equals(roughness, 0.0f)) {
		std::unique_ptr<BXDF> refr(new RefractionBTDF(color, m_fresnel.get(), IORin, IORout));
		bsdf.addBXDF(refr);
	}
	else {
		std::unique_ptr<BXDF> mcr(new MicrofacetBTDF(m_distribution.get(), m_fresnel.get(),
			roughness, color, IORin, IORout));
		bsdf.addBXDF(mcr);
	}
	return bsdf;
}

