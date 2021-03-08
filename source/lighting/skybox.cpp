#include "lighting/skybox.h"
#include "tools/constants.h"
#include "tools/shading-functions.h"

#include <iostream>
Spectrum Skybox::emission(const Vector3& direction) const {
	Vector3 rotated = m_rotation.applyVector(direction);
	SurfacePoint uvp;
	uvp.uv.x = (std::atan2(direction.y, direction.x) + PI) * INV_TWO_PI;
	uvp.uv.y = (1.0f + rotated.z) * 0.5f;
	uvp.dUVdx.x = 0.0f;
	uvp.dUVdx.y = 0.0f;
	uvp.dUVdy.x = 0.0f;
	uvp.dUVdy.y = 0.0f;
	return m_skyboxTexture->sample(uvp) * m_intensity;
}

float Skybox::pdf(const Vector3& direction) const {
	return INV_FOUR_PI;
}

SkyboxSample Skybox::sample(Sampler* sampler) const {
	Vector3 sampledDirection = shading::uniformSampleSphere(sampler->getSample2D());
	return SkyboxSample(emission(sampledDirection), sampledDirection, INV_TWO_PI);
}

