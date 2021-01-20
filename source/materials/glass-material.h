#pragma once

class GlassMaterial;

#include "materials/material.h"
#include "textures/texture.h"

#include <memory>

class GlassMaterial : public Material {
private:
	const Texture* m_color;
	const Texture* m_roughness;
	float m_materialIOR;
	float m_airIOR;

public:
	GlassMaterial() = delete;
	GlassMaterial(const GlassMaterial& other) = delete;
	GlassMaterial(const Texture* color, const Texture* roughness, float materialIOR, float airIOR);
	GlassMaterial(const Texture* color, const Texture* roughness, float materialIOR);

	std::unique_ptr<Shader> createShader(const SurfacePoint& point, const Vector3& wo) const;
};

