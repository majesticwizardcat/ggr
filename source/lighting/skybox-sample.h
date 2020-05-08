#pragma once

class Skybox;

#include "primitives/vector.h"
#include "spectra/spectrum.h"

class SkyboxSample {
public:
	Spectrum emission;
	Vector3 sampledDirection;
	float pdf;

	SkyboxSample();
	SkyboxSample(const SkyboxSample& other);
	SkyboxSample(const Spectrum& emission, const Vector3& direction, float pdf);
};
