#pragma once

class BlendMaterial;

#include "materials/material.h"
#include "textures/texture.h"

#include <memory>

class BlendMaterial : public Material {
private:
	const Texture* m_diffuse;
	const Texture* m_glossy;
	const Texture* m_roughness;
	float m_blend;

public:
	BlendMaterial() = delete;
	BlendMaterial(const BlendMaterial& other) = delete;
	BlendMaterial(const Texture* diffuse, const Texture* glossy, const Texture* roughness) :
		BlendMaterial(diffuse, glossy, roughness, 0.5f) { }
	BlendMaterial(const Texture* diffuse, const Texture* glossy, const Texture* roughness, float blend) :
		m_diffuse(diffuse), m_glossy(glossy), m_roughness(roughness), m_blend(blend) { }


	Shader* createShader(const SurfacePoint& point, const Vector3& wo, StackAllocator& alloc) const;
};
