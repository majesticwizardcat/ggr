#pragma once

class MirrorMaterial;

#include "materials/material.h"
#include "textures/texture.h"
#include "spectra/spectrum.h"
#include "bsdfs/fresnel.h"

#include <memory>

class MirrorMaterial : public Material {
private:
	const Texture* m_color;
	std::unique_ptr<Fresnel> m_fresnel;

public:
	MirrorMaterial() = delete;
	MirrorMaterial(const MirrorMaterial& other) = delete;
	MirrorMaterial(const Texture* color);

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};

