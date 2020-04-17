#pragma once

class PathIntegrator;

#include "integrators/film-sampling-integrator.h"

class PathIntegrator : public FilmSamplingIntegrator {
public:
	PathIntegrator();
	PathIntegrator(const PathIntegrator& other);

	std::unique_ptr<Integrator> clone() const;
	Spectrum traceRay(const Ray& ray, const Scene& scene, Camera* camera, Sampler* sampler);
};

