#pragma once

class MatteMaterial;

#include "materials/material.h"
#include "textures/texture.h"

class MatteMaterial : public Material {
private:
	const Texture* m_diffuseColor;
	
public:
	MatteMaterial() = delete;
	MatteMaterial(const MatteMaterial& other) = delete;
	MatteMaterial(const Texture* diffuseColor) : Material(), m_diffuseColor(diffuseColor) { }

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};

