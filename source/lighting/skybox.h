#pragma once

class Skybox;

#include "textures/texture.h"
#include "samplers/sampler.h"
#include "primitives/transformation.h"
#include "lighting/skybox-sample.h"

#include <memory>

class Skybox {
private:
	const Texture* m_skyboxTexture;
	float m_intensity;
	Transformation m_rotation;

public:
	Skybox() = delete;
	Skybox(const Skybox& other) : m_skyboxTexture(other.m_skyboxTexture),
		m_intensity(other.m_intensity), m_rotation(other.m_rotation) { }
	Skybox(const Texture* skyboxTexture, float intensity, const Transformation& rotation)
		: m_skyboxTexture(skyboxTexture), m_intensity(intensity), m_rotation(rotation) { }

	Spectrum emission(const Vector3& direction) const;
	float pdf(const Vector3& direction) const;
	SkyboxSample sample(Sampler* sampler) const;
};

