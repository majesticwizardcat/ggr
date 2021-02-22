#pragma once

class PathIntegrator;

#include "integrators/film-sampling-integrator.h"

class PathIntegrator : public FilmSamplingIntegrator {
public:
	PathIntegrator() { }
	PathIntegrator(const PathIntegrator& other) = delete;

	void renderPixel(const Scene* scene, const Camera* camera, Film* film,
		Sampler* sampler, const Point2& pixel, unsigned int samples) const;
};
