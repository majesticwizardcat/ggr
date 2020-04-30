#pragma once

class PlasticMaterial;

#include "materials/material.h"
#include "textures/texture.h"
#include "bsdfs/microfacet-distribution.h"
#include "bsdfs/fresnel.h"

#include <memory>

class PlasticMaterial : public Material {
private:
	std::unique_ptr<MicrofacetDistribution> m_distribution;
	std::unique_ptr<Fresnel> m_fresnel;
	std::shared_ptr<Texture> m_color;
	std::shared_ptr<Texture> m_roughness;
	float m_ratio;

public:
	PlasticMaterial() = delete;
	PlasticMaterial(const PlasticMaterial& other);
	PlasticMaterial(const std::shared_ptr<Texture> color,
		const std::shared_ptr<Texture> roughness);
	PlasticMaterial(const std::shared_ptr<Texture> color,
		const std::shared_ptr<Texture> roughness, float ratio);

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};

