#pragma once

class GlassMaterial;

#include "materials/material.h"
#include "bsdfs/microfacet-distribution.h"
#include "bsdfs/fresnel.h"
#include "textures/texture.h"

#include <memory>

class GlassMaterial : public Material {
private:
	const Texture* m_color;
	const Texture* m_roughness;
	std::unique_ptr<Fresnel> m_fresnel;
	std::unique_ptr<MicrofacetDistribution> m_distribution;
	float m_materialIOR;
	float m_airIOR;

public:
	GlassMaterial() = delete;
	GlassMaterial(const GlassMaterial& other) = delete;
	GlassMaterial(const Texture* color, const Texture* roughness, float materialIOR, float airIOR);
	GlassMaterial(const Texture* color, const Texture* roughness, float materialIOR);

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};

