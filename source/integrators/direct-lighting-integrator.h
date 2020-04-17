#pragma once

class DirectLightingIntegrator;

#include "film-sampling-integrator.h"

class DirectLightingIntegrator : public FilmSamplingIntegrator {
public:
	DirectLightingIntegrator();
	DirectLightingIntegrator(const DirectLightingIntegrator& other);

	std::unique_ptr<Integrator> clone() const;
	Spectrum traceRay(const Ray& ray, const Scene& scene, Camera* camera, Sampler* sampler);
};

