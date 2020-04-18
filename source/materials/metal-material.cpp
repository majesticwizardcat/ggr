#include "materials/metal-material.h"
#include "bsdfs/microfacet-brdf.h"
#include "bsdfs/specular-brdf.h"
#include "tools/util.h"

MetalMaterial::MetalMaterial() { }
MetalMaterial::MetalMaterial(const MetalMaterial& other) :
	m_color(other.m_color), m_roughness(other.m_roughness) {
	m_fresnel = std::unique_ptr<Fresnel>(new FresnelSchlick(Spectrum(1.0f)));
	m_distribution = std::unique_ptr<GGXDistribution>(new GGXDistribution());
}

MetalMaterial::MetalMaterial(const std::shared_ptr<Texture>& color,
	const std::shared_ptr<Texture>& roughness) : m_color(color), m_roughness(roughness) {
	m_fresnel = std::unique_ptr<Fresnel>(new FresnelSchlick(Spectrum(1.0f)));
	m_distribution = std::unique_ptr<GGXDistribution>(new GGXDistribution());
}

BSDF MetalMaterial::createBSDF(const SurfacePoint& point, const Vector3& wo) const {
	BSDF bsdf(point);
	float roughness = util::clamp(m_roughness->sample(point).value(), 0.0f, 1.0f);
	roughness *= roughness;

	if (util::equals(roughness, 0.0f)) {
		std::unique_ptr<BXDF> spec(new SpecularBRDF(m_color->sample(point), m_fresnel.get()));
		bsdf.addBXDF(spec);
	}
	else {
		std::unique_ptr<BXDF> mc(new MicrofacetBRDF(m_distribution.get(),
			m_fresnel.get(), roughness, m_color->sample(point)));
		bsdf.addBXDF(mc);
	}
	return bsdf;
}

