#pragma once

class FilmSamplingIntegrator;

#include "integrators/integrator.h"
#include "film/film.h"

#include <mutex>
#include <memory>

class FilmSamplingIntegrator : public Integrator {
private:
	std::vector<UnfilteredFilmTile> m_tiles;
	std::mutex m_tilesLock;
	std::mutex m_filmLock;
	size_t m_initialSize;

public:
	FilmSamplingIntegrator() { }
	FilmSamplingIntegrator(const Integrator& other) = delete;

	void setup(const Scene* scene, const Camera* camera, Sampler* sampler, const RenderSettings& settings);
	bool render(const Scene* scene, const Camera* camera, Sampler* sampler, const RenderSettings& settings);
	Image combine();
	float getCompletion() const;

	virtual void reset();
	virtual Spectrum traceRay(Ray* ray, const Scene* scene, const Camera* camera, Sampler* sampler) = 0;
};

