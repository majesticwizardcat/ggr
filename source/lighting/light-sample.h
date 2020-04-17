#pragma once

class LightSample;

#include "spectra/spectrum.h"
#include "intersection/surface-point.h"

class LightSample {
public:
	Spectrum emission;
	SurfacePoint sampledPoint;
	float pdf;

	LightSample();
	LightSample(const LightSample& other);
	LightSample(const Spectrum& emission, const SurfacePoint& sampledPoint, float pdf);
};

