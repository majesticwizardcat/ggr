#pragma once

class TransparentMaterial;

#include "materials/material.h"
#include "textures/texture.h"

#include <memory>

class TransparentMaterial : public Material {
private:
	const Texture* m_color;
	float m_materialIOR;
	float m_airIOR;

public:
	TransparentMaterial() = delete;
	TransparentMaterial(const TransparentMaterial& other) = delete;
	TransparentMaterial(const Texture* color, float materialIOR, float airIOR);
	TransparentMaterial(const Texture* color, float materialIOR);

	std::unique_ptr<Shader> createShader(const SurfacePoint& point, const Vector3& wo) const;
};
