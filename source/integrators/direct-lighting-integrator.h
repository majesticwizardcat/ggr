#pragma once

class DirectLightingIntegrator;

#include "film-sampling-integrator.h"

class DirectLightingIntegrator : public FilmSamplingIntegrator {
public:
	DirectLightingIntegrator() { }
	DirectLightingIntegrator(const DirectLightingIntegrator& other) = delete;

	Spectrum traceRay(Ray* ray, const Scene* scene, const Camera* camera, Sampler* sampler);
};

