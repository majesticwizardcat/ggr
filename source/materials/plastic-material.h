#pragma once

class PlasticMaterial;

#include "materials/material.h"
#include "textures/texture.h"
#include "bsdfs/microfacet-distribution.h"
#include "bsdfs/fresnel.h"

#include <memory>

class PlasticMaterial : public Material {
private:
	const Texture* m_color;
	const Texture* m_roughness;
	std::unique_ptr<MicrofacetDistribution> m_distribution;
	std::unique_ptr<Fresnel> m_fresnel;
	float m_ratio;

public:
	PlasticMaterial() = delete;
	PlasticMaterial(const PlasticMaterial& other) = delete;
	PlasticMaterial(const Texture* color, const Texture* roughness);
	PlasticMaterial(const Texture* color, const Texture* roughness, float ratio);

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};

