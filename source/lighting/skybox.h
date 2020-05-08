#pragma once

class Skybox;

#include "textures/texture.h"
#include "samplers/sampler.h"
#include "primitives/transformation.h"
#include "lighting/skybox-sample.h"

#include <memory>

class Skybox {
private:
	std::shared_ptr<Texture> m_skyboxTexture;
	float m_intensity;
	Transformation m_rotation;

public:
	Skybox();
	Skybox(const Skybox& other);
	Skybox(const std::shared_ptr<Texture> skyboxTexture);
	Skybox(const std::shared_ptr<Texture> skyboxTexture,
		float intensity);
	Skybox(const std::shared_ptr<Texture> skyboxTexture,
		float intensity, const Transformation& rotation);

	Spectrum emission(const Vector3& direction) const;
	float pdf(const Vector3& direction) const;
	SkyboxSample sample(Sampler* sampler) const;
};

