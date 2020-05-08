#include "lighting/skybox-sample.h"

SkyboxSample::SkyboxSample() : pdf(0.0f) { }
SkyboxSample::SkyboxSample(const SkyboxSample& other) :
	SkyboxSample(other.emission, other.sampledDirection, other.pdf) { }
SkyboxSample::SkyboxSample(const Spectrum& emission, const Vector3& direction, float pdf) :
	emission(emission), sampledDirection(direction), pdf(pdf) { }

