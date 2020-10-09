#include "materials/glass-material.h"
#include "bsdfs/microfacet-btdf.h"
#include "bsdfs/refraction-btdf.h"
#include "tools/util.h"

GlassMaterial::GlassMaterial(const Texture* color, const Texture* roughness, float materialIOR, float airIOR) :
	Material(), m_color(color), m_roughness(roughness), m_materialIOR(materialIOR), m_airIOR(airIOR) {
	m_distribution = std::make_unique<GGXDistribution>();
	m_fresnel = std::make_unique<FresnelCT>();
}

GlassMaterial::GlassMaterial(const Texture* color, const Texture* roughness, float materialIOR) :
	GlassMaterial(color, roughness, materialIOR, 1.0f) { }

BSDF GlassMaterial::createBSDF(const SurfacePoint& point, const Vector3& wo) const {
	BSDF bsdf(point);
	float IORin;
	float IORout;
	if (glm::dot(point.geometricNormal, wo) > 0) {
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
		std::unique_ptr<BXDF> refr = std::make_unique<RefractionBTDF>(color, m_fresnel.get(), IORin, IORout);
		bsdf.addBXDF(refr);
	}
	else {
		std::unique_ptr<BXDF> mcr = std::make_unique<MicrofacetBTDF>(m_distribution.get(), m_fresnel.get(),
			roughness, color, IORin, IORout);
		bsdf.addBXDF(mcr);
	}
	return bsdf;
}

