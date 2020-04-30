#pragma once

class TransparentMaterial;

#include "materials/material.h"
#include "textures/texture.h"
#include "bsdfs/fresnel.h"

#include <memory>

class TransparentMaterial : public Material {
private:
	std::unique_ptr<Fresnel> m_fresnel;
	std::shared_ptr<Texture> m_color;
	float m_materialIOR;
	float m_airIOR;

public:
	TransparentMaterial() = delete;
	TransparentMaterial(const TransparentMaterial& other);
	TransparentMaterial(const std::shared_ptr<Texture>& color, float materialIOR,
		float airIOR);
	TransparentMaterial(const std::shared_ptr<Texture>& color, float materialIOR);

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};

