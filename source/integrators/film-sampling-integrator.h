#pragma once

class FilmSamplingIntegrator;

#include "integrators/integrator.h"
#include "film/film.h"

#include <mutex>
#include <memory>

class FilmSamplingIntegrator : public Integrator {
private:
	std::unique_ptr<Film> m_frame;
	std::vector<UnfilteredFilmTile> m_tiles;
	std::mutex m_tilesLock;
	std::mutex m_filmLock;
	int m_initialSize;

public:
	FilmSamplingIntegrator();
	FilmSamplingIntegrator(const Integrator& other);

	void setup(const Scene& scene, Camera* camera, Sampler* sampler, const RenderSettings& settings);
	bool render(const Scene& scene, Camera* camera, Sampler* sampler, const RenderSettings& settings);
	Image combine();
	float getCompletion() const;

	virtual std::unique_ptr<Integrator> clone() const = 0;
	virtual Spectrum traceRay(const Ray& ray, const Scene& scene, Camera* camera, Sampler* sampler) = 0;
};

