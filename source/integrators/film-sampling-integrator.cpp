#include "integrators/film-sampling-integrator.h"

#include <cmath>
#include <iostream>

void FilmSamplingIntegrator::setup(const Scene* scene, const Camera* camera, Film* film,
	Sampler* sampler, const RenderSettings& settings) {
	m_tiles = film->splitToTiles(settings.tileSize);
	m_initialSize = m_tiles.size();
}

void FilmSamplingIntegrator::render(
		const Scene* scene, const Camera* camera, Film* film,
		Sampler* sampler, const RenderSettings& settings) {
	std::unique_ptr<Sampler> samplerClone = sampler->clone();
	while (true) {
		std::cout << 
			"\rCompeleted: " << getCompletion() 
			<< " %                   ";
		std::cout.flush();
		m_stackLock.lock();
		if (m_tiles.empty()) {
			m_stackLock.unlock();
			break;
		}
		FilmBounds tile = std::move(m_tiles.back());
		m_tiles.pop_back();
		m_stackLock.unlock();
		// I am still not conviced this can not be extracted
		for (int x = tile.start.first; x < tile.end.first; ++x) {
			for (int y = tile.start.second; y < tile.end.second; ++y) {
				Point2 nextPixel((float)x, (float)y);
				samplerClone->createCameraSamples(nextPixel, settings.samples);
				renderPixel(scene, camera, film, samplerClone.get(), nextPixel, settings.samples);
			}
		}
	}
}

void FilmSamplingIntegrator::reset() {
	m_initialSize = 0;
	m_tiles.clear();
}

float FilmSamplingIntegrator::getCompletion() const {
	return 100.0f * ((float) (m_initialSize - m_tiles.size()) / (float) m_initialSize);
}
