#pragma once

class PlasticMaterial;

#include "materials/material.h"
#include "textures/texture.h"

#include <memory>

class PlasticMaterial : public Material {
private:
	const Texture* m_color;
	const Texture* m_roughness;
	float m_blend;

public:
	PlasticMaterial() = delete;
	PlasticMaterial(const PlasticMaterial& other) = delete;
	PlasticMaterial(const Texture* color, const Texture* roughness);
	PlasticMaterial(const Texture* color, const Texture* roughness, float blend);

	std::unique_ptr<Shader> createShader(const SurfacePoint& point, const Vector3& wo) const;
};

