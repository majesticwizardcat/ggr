#pragma once

class EmissionMaterial;

#include "materials/material.h"
#include "textures/texture.h"
#include "shaders/emission-shader.h"

#include <memory>

class EmissionMaterial : public Material {
private:
	const Texture* m_emission;
	float m_intensity;

public:
	EmissionMaterial() = delete;
	EmissionMaterial(const EmissionMaterial& other) : 
		Material(other), m_emission(other.m_emission),
		m_intensity(other.m_intensity) { }

	// I noticed this all, group together things vertically/horizontally
	EmissionMaterial(const Texture* emission, float intensity) : 
		Material(), m_emission(emission), m_intensity(intensity) { }

	std::unique_ptr<Shader> createShader(const SurfacePoint& point, const Vector3& wo) const {
		return std::make_unique<EmissionShader>(
				point.shadingNormal, point.tangent, point.bitangent,
				m_emission->sample(point) * m_intensity);
	}
};

