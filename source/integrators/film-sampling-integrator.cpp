#include "integrators/film-sampling-integrator.h"

#include <cmath>
#include <iostream>

void FilmSamplingIntegrator::setup(const Scene* scene, const Camera* camera, Film* film,
	Sampler* sampler, const RenderSettings& settings) {
	m_filmTiles = film->splitToTiles(settings.tileSize);
	m_initialSize = m_filmTiles.size();
}

void FilmSamplingIntegrator::render(const Scene* scene, const Camera* camera, Film* film,
		Sampler* sampler, const RenderSettings& settings) {
	CameraSample cameraSample;
	Ray ray;
	Spectrum radiance;
	std::unique_ptr<Sampler> samplerClone = sampler->clone();

	while(true) {
		std::cout << "\rCompleted: " << (int) (getCompletion() * 100.0f) << "%";
		m_boundsLock.lock();
		if (m_filmTiles.empty()) {
			m_boundsLock.unlock();
			return;
		}
		FilmBounds bounds = m_filmTiles.back();
		m_filmTiles.pop_back();
		m_boundsLock.unlock();
		auto tile = film->createTile(bounds);

		for (unsigned int x = tile->sampleBounds.start.first; x < tile->sampleBounds.end.first; ++x) {
			for (unsigned int y = tile->sampleBounds.start.second; y < tile->sampleBounds.end.second; ++y) {
				samplerClone->createCameraSamples(Point2((float)x, (float)y), settings.samples);
				for (unsigned int s = 0; s < settings.samples; ++s) {
					cameraSample = samplerClone->getCameraSample(Point2(x, y));
					camera->generateRay(&ray, cameraSample);
					radiance = traceRay(&ray, scene, camera, samplerClone.get());
					tile->addSample(x, y, cameraSample.filmPosition, radiance, ray.weight);
				}
			}
		}
		film->mergeRenderTile(tile.get());
	}
}

void FilmSamplingIntegrator::reset() {
	m_filmTiles.clear();
	m_initialSize = 0;
}

float FilmSamplingIntegrator::getCompletion() const {
	return (float) (m_initialSize - m_filmTiles.size()) / (float) m_initialSize;
}
