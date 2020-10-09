#pragma once

class EmissionMaterial;

#include "materials/material.h"
#include "textures/texture.h"

#include <memory>

class EmissionMaterial : public Material {
private:
	const Texture* m_emission;
	float m_intensity;

public:
	EmissionMaterial() = delete;
	EmissionMaterial(const EmissionMaterial& other) : Material(other), m_emission(other.m_emission),
		m_intensity(other.m_intensity) { }
	EmissionMaterial(const Texture* emission, float intensity) : Material(),
		m_emission(emission), m_intensity(intensity) { }

	BSDF createBSDF(const SurfacePoint& point, const Vector3& wo) const;
};

