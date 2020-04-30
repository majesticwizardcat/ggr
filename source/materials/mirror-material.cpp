#include "materials/mirror-material.h"
#include "bsdfs/specular-brdf.h"

MirrorMaterial::MirrorMaterial(const MirrorMaterial& other) : m_color(other.m_color) {
	m_fresnel = std::make_unique<FresnelSchlick>(Spectrum(1.0f));
}

MirrorMaterial::MirrorMaterial(const std::shared_ptr<Texture>& color) : m_color(color) {
	m_fresnel = std::make_unique<FresnelSchlick>(Spectrum(1.0f));
}

BSDF MirrorMaterial::createBSDF(const SurfacePoint& point, const Vector3& wo) const {
	std::unique_ptr<BXDF> spec = std::make_unique<SpecularBRDF>(m_color->sample(point), m_fresnel.get());
	BSDF bsdf(point);
	bsdf.addBXDF(spec);
	return bsdf;
}

