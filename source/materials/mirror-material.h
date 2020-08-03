#pragma once

class MirrorMaterial;

#include "materials/material.h"
#include "textures/texture.h"
#include "spectra/spectrum.h"
#include "bsdfs/fresnel.h"

#include <memory>

class MirrorMaterial : public Material {
private:
	std::unique_ptr<Fresnel> m_fresnel;
	std::shared_ptr<Texture> m_color;

public:
	MirrorMaterial() = delete;
	MirrorMaterial(const MirrorMaterial& other);
	MirrorMaterial(const std::shared_ptr<Texture>& color);

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};

