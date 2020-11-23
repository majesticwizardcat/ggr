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
	std::vector<UnfilteredSample> boundSamples;
	CameraSample cameraSample;
	Ray ray;
	Spectrum radiance;
	std::unique_ptr<Sampler> samplerClone = sampler->clone();

	while(true) {
		std::cout << "\rCompleted: " << (int) getCompletion() * 100 << "%";
		m_boundsLock.lock();
		if (m_filmTiles.empty()) {
			m_boundsLock.unlock();
			return;
		}
		FilmBounds bounds = m_filmTiles.back();
		m_filmTiles.pop_back();
		m_boundsLock.unlock();

		float padX = std::ceil(2.0f + settings.filter->getWidth());
		float padY = std::ceil(2.0f + settings.filter->getHeight());
		Point2 freeBoxStart(bounds.start.first + padX, bounds.start.second + padY);
		Point2 freeBoxEnd(std::max(freeBoxStart.x, bounds.end.first - padX),
				  std::max(freeBoxStart.y, bounds.end.second - padY));
		for (unsigned int x = bounds.start.first; x < bounds.end.first; ++x) {
			for (unsigned int y = bounds.start.second; y < bounds.end.second; ++y) {
				samplerClone->createCameraSamples(Point2(x, y), settings.samples);
				for (unsigned int s = 0; s < settings.samples; ++s) {
					cameraSample = samplerClone->getCameraSample(Point2(x, y));
					camera->generateRay(&ray, cameraSample);
					radiance = traceRay(&ray, scene, camera, samplerClone.get());
					if (x > freeBoxStart.x && x < freeBoxEnd.x
						&& y > freeBoxStart.y && y < freeBoxEnd.y) {
						film->addUnfilteredSample(cameraSample.filmPosition, radiance, ray.weight);
					}

					else {
						boundSamples.push_back(UnfilteredSample(cameraSample.filmPosition, radiance, ray.weight));
					}
				}
			}
		}
		if (boundSamples.size() > 0 && m_filmLock.try_lock()) {
			film->addUnfilteredSampleVector(&boundSamples);
			m_filmLock.unlock();
		}
	}
}

void FilmSamplingIntegrator::reset() {
	m_filmTiles.clear();
	m_initialSize = 0;
}

float FilmSamplingIntegrator::getCompletion() const {
	return (float) (m_initialSize - m_filmTiles.size()) / (float) m_initialSize;
}
