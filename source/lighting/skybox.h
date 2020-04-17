#pragma once

class Skybox;
class SkyboxSample;

#include "textures/texture.h"
#include "samplers/sampler.h"
#include "primitives/transformation.h"

#include <memory>

class SkyboxSample {
public:
	Spectrum emission;
	Vector3 sampledDirection;
	float pdf;

	SkyboxSample();
	SkyboxSample(const SkyboxSample& other);
	SkyboxSample(const Spectrum& emission, const Vector3& direction, float pdf);
};

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

