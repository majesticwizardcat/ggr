#pragma once

class FilmSamplingIntegrator;

#include "integrators/integrator.h"
#include "film/film.h"

#include <mutex>
#include <memory>

class FilmSamplingIntegrator : public Integrator {
private:
	std::vector<FilmBounds> m_filmTiles;
	std::mutex m_filmLock;
	std::mutex m_boundsLock;
	size_t m_initialSize;

public:
	FilmSamplingIntegrator() { }
	FilmSamplingIntegrator(const Integrator& other) = delete;

	void setup(const Scene* scene, const Camera* camera, Sampler* sampler, const RenderSettings& settings);
	void render(const Scene* scene, const Camera* camera, Sampler* sampler, const RenderSettings& settings);
	Image combine();
	float getCompletion() const;

	virtual void reset();
	virtual Spectrum traceRay(Ray* ray, const Scene* scene, const Camera* camera, Sampler* sampler) = 0;
};

