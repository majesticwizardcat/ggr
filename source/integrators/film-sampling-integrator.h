#pragma once

class FilmSamplingIntegrator;

#include "integrators/integrator.h"
#include "film/film.h"

#include <mutex>
#include <memory>
#include <atomic>

class FilmSamplingIntegrator : public Integrator {
private:
	std::atomic<int> m_nextWorkerIndex;
	std::vector<std::vector<Point2>> m_renderArrays;
	unsigned int m_initialSize;
	unsigned int m_pixelsRendered;

public:
	FilmSamplingIntegrator() : m_nextWorkerIndex(0), m_initialSize(0),
		m_pixelsRendered(0) { }
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

