#pragma once

class EmissionMaterial;

#include "materials/material.h"
#include "textures/texture.h"

#include <memory>

class EmissionMaterial : public Material {
private:
	std::shared_ptr<Texture> m_emission;
	float m_intensity;

public:
	EmissionMaterial() = delete;
	EmissionMaterial(const EmissionMaterial& other);
	EmissionMaterial(const std::shared_ptr<Texture> emission, float intensity);

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};

