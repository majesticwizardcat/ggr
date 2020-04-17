#pragma once

class GlassMaterial;

#include "materials/material.h"
#include "bsdfs/microfacet-distribution.h"
#include "bsdfs/fresnel.h"
#include "textures/texture.h"

#include <memory>

class GlassMaterial : public Material {
private:
	std::shared_ptr<Texture> m_color;
	std::shared_ptr<Texture> m_roughness;
	std::unique_ptr<Fresnel> m_fresnel;
	std::unique_ptr<MicrofacetDistribution> m_distribution;
	float m_materialIOR;
	float m_airIOR;

public:
	GlassMaterial();
	GlassMaterial(const GlassMaterial& other);
	GlassMaterial(const std::shared_ptr<Texture>& color,
		const std::shared_ptr<Texture>& roughness, float materialIOR,
		float airIOR);
	GlassMaterial(const std::shared_ptr<Texture>& color,
		const std::shared_ptr<Texture>& roughness, float materialIOR);

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};

