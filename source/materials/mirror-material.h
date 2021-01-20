#pragma once

class MirrorMaterial;

#include "materials/material.h"
#include "textures/texture.h"
#include "spectra/spectrum.h"
#include "shaders/specular-shader.h"

#include <memory>

class MirrorMaterial : public Material {
private:
	const Texture* m_color;

public:
	MirrorMaterial() = delete;
	MirrorMaterial(const MirrorMaterial& other) = delete;
	MirrorMaterial(const Texture* color) : m_color(color) { }

	std::unique_ptr<Shader> createShader(const SurfacePoint& point, const Vector3& wo) const {
		return std::make_unique<SpecularShader>(point.shadingNormal, point.tangent, point.bitangent,
			m_color->sample(point));
	}
};
