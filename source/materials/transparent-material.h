#pragma once

class TransparentMaterial;

#include "materials/material.h"
#include "textures/texture.h"
#include "bsdfs/fresnel.h"

#include <memory>

class TransparentMaterial : public Material {
private:
	const Texture* m_color;
	std::unique_ptr<Fresnel> m_fresnel;
	float m_materialIOR;
	float m_airIOR;

public:
	TransparentMaterial() = delete;
	TransparentMaterial(const TransparentMaterial& other) = delete;
	TransparentMaterial(const Texture* color, float materialIOR, float airIOR);
	TransparentMaterial(const Texture* color, float materialIOR);

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};

