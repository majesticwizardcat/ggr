#pragma once

class MetalMaterial;

#include "materials/material.h"
#include "bsdfs/fresnel.h"
#include "bsdfs/microfacet-distribution.h"
#include "textures/texture.h"

#include <memory>

class MetalMaterial : public Material {
private:
	std::shared_ptr<Texture> m_color;
	std::shared_ptr<Texture> m_roughness;
	std::unique_ptr<Fresnel> m_fresnel;
	std::unique_ptr<GGXDistribution> m_distribution;

public:
	MetalMaterial() = delete;
	MetalMaterial(const MetalMaterial& other);
	MetalMaterial(const std::shared_ptr<Texture>& color,
		const std::shared_ptr<Texture>& roughness);

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};
