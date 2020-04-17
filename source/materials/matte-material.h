#pragma once

class MatteMaterial;

#include "materials/material.h"
#include "textures/texture.h"

#include <memory>

class MatteMaterial : public Material {
private:
	std::shared_ptr<Texture> m_diffuseColor;
	
public:
	MatteMaterial();
	MatteMaterial(const MatteMaterial& other);
	MatteMaterial(const std::shared_ptr<Texture>& diffuseColor);

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};

