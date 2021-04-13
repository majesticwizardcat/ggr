#pragma once

class FilmSamplingIntegrator;

#include "integrators/integrator.h"
#include "film/film.h"

#include <mutex>
#include <memory>

class FilmSamplingIntegrator : public Integrator {
private:
	std::mutex m_stackLock;
	std::vector<FilmBounds> m_tiles;
	int m_initialSize;

public:
	FilmSamplingIntegrator() : m_initialSize(0) { }
	FilmSamplingIntegrator(const Integrator& other) = delete;

	void setup(const Scene* scene, const Camera* camera, Film* film,
		Sampler* sampler, const RenderSettings& settings);
	void render(const Scene* scene, const Camera* camera, Film* film,
		Sampler* sampler, const RenderSettings& settings);
	float getCompletion() const;

	virtual void reset();
	virtual void renderPixel(const Scene* scene, const Camera* camera, Film* film,
		Sampler* sampler, const Point2& pixel, unsigned int samples) const = 0;
};

