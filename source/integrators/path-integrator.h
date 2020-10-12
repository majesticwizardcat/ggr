#pragma once

class PathIntegrator;

#include "integrators/film-sampling-integrator.h"

class PathIntegrator : public FilmSamplingIntegrator {
public:
	PathIntegrator() { }
	PathIntegrator(const PathIntegrator& other) = delete;

	Spectrum traceRay(Ray* ray, const Scene* scene, const Camera* camera, Sampler* sampler);
};

