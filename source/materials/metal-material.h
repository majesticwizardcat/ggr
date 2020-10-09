#pragma once

class MetalMaterial;

#include "materials/material.h"
#include "bsdfs/fresnel.h"
#include "bsdfs/microfacet-distribution.h"
#include "textures/texture.h"

#include <memory>

class MetalMaterial : public Material {
private:
	const Texture* m_color;
	const Texture* m_roughness;
	std::unique_ptr<Fresnel> m_fresnel;
	std::unique_ptr<GGXDistribution> m_distribution;

public:
	MetalMaterial() = delete;
	MetalMaterial(const MetalMaterial& other) = delete;
	MetalMaterial(const Texture* color, const Texture* roughness);

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};
