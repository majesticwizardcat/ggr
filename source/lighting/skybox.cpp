#include "lighting/skybox.h"
#include "tools/constants.h"
#include "tools/shading-functions.h"

Spectrum Skybox::emission(const Vector3& direction) const {
	Point2 spherical = shading::toSpherical(m_rotation.applyInversePoint(direction));
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

