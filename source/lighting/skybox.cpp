#include "lighting/skybox.h"
#include "tools/constants.h"
#include "tools/shading-functions.h"

SkyboxSample::SkyboxSample() : pdf(0.0f) { }
SkyboxSample::SkyboxSample(const SkyboxSample& other) :
	SkyboxSample(other.emission, other.sampledDirection, other.pdf) { }
SkyboxSample::SkyboxSample(const Spectrum& emission, const Vector3& direction, float pdf) :
	emission(emission), sampledDirection(direction), pdf(pdf) { }

Skybox::Skybox() { }
Skybox::Skybox(const Skybox& other) : Skybox(other.m_skyboxTexture) { }
Skybox::Skybox(const std::shared_ptr<Texture> skyboxTexture) :
	m_skyboxTexture(skyboxTexture), m_intensity(0.0f) { }
Skybox::Skybox(const std::shared_ptr<Texture> skyboxTexture, float intensity) :
	m_skyboxTexture(skyboxTexture), m_intensity(intensity) { }
Skybox::Skybox(const std::shared_ptr<Texture> skyboxTexture, float intensity,
	const Transformation& rotation) : m_skyboxTexture(skyboxTexture),
	m_intensity(intensity), m_rotation(rotation) { }

Spectrum Skybox::emission(const Vector3& direction) const {
	Point2 spherical = shading::toSpherical(m_rotation.applyInverse(direction));
	SurfacePoint uvp;
	uvp.uv = Point2(spherical.x * INV_TWO_PI, spherical.y * INV_PI);
	return m_skyboxTexture->sample(uvp) * m_intensity;
}

float Skybox::pdf(const Vector3& direction) const {
	return INV_TWO_PI;
}

SkyboxSample Skybox::sample(Sampler* sampler) const {
	Vector3 sampledDirection = shading::uniformSampleSphere(sampler->getSample2D());
	return SkyboxSample(emission(sampledDirection), sampledDirection, INV_TWO_PI);
}

