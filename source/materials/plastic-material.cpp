#include "materials/plastic-material.h"
#include "bsdfs/microfacet-brdf.h"
#include "bsdfs/diffuse-brdf.h"
#include "bsdfs/specular-brdf.h"
#include "tools/util.h"

PlasticMaterial::PlasticMaterial() { }
PlasticMaterial::PlasticMaterial(const PlasticMaterial& other) :
	m_color(other.m_color), m_roughness(other.m_roughness) { }
PlasticMaterial::PlasticMaterial(const std::shared_ptr<Texture> color,
	const std::shared_ptr<Texture> roughness) :
	PlasticMaterial(color, roughness, 0.5f) { }
PlasticMaterial::PlasticMaterial(const std::shared_ptr<Texture> color,
		const std::shared_ptr<Texture> roughness, float ratio) :
	m_color(color), m_roughness(roughness), m_ratio(ratio) {
	m_fresnel = std::unique_ptr<Fresnel>(new FresnelSchlick(Spectrum(1.0f)));
	m_distribution = std::unique_ptr<MicrofacetDistribution>(new GGXDistribution());
}

BSDF PlasticMaterial::createBSDF(const SurfacePoint& point, const Vector3& wo) const {
	BSDF bsdf(point);
	Spectrum color = m_color->sample(point);
	float roughness = util::clamp(m_roughness->sample(point).value(), 0.0f, 1.0f);
	roughness *= roughness;
	std::unique_ptr<BXDF> diffuse(new DiffuseBRDF(color * (1.0f - m_ratio)));
	std::unique_ptr<BXDF> gloss;
	
	if (roughness > 0) {
		gloss = std::unique_ptr<BXDF>(new MicrofacetBRDF(m_distribution.get(),
			m_fresnel.get(), roughness, Spectrum(m_ratio)));
	}
	else {
		gloss = std::unique_ptr<BXDF>(new SpecularBRDF(Spectrum(m_ratio), m_fresnel.get()));
	}

	bsdf.addBXDF(diffuse);
	bsdf.addBXDF(gloss);
	return bsdf;
}

