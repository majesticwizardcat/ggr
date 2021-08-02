#pragma once

class MetalMaterial;

#include "materials/material.h"
#include "textures/texture.h"

#include <memory>

class MetalMaterial : public Material {
private:
	const Texture* m_color;
	const Texture* m_roughness;

public:
	MetalMaterial() = delete;
	MetalMaterial(const MetalMaterial& other) = delete;
	MetalMaterial(const Texture* color, const Texture* roughness);

	Shader* createShader(const SurfacePoint& point, const Vector3& wo, StackAllocator& alloc) const;
};
